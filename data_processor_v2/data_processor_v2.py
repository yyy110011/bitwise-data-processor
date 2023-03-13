import numpy
import os
import boto3
from sqlalchemy import create_engine, Column, Integer, LargeBinary, String
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker

Base = declarative_base()

class DataItem(Base):
    __tablename__ = 'data'

    id = Column(Integer, primary_key=True)
    channel_id = Column(String(255))
    data = Column(LargeBinary)
    start_timestamp = Column(Integer)
    end_timestamp = Column(Integer)

    def __repr__(self):
        return f"DataItem(id={self.id}, channel_id='{self.channel_id}', data=numpy.frombuffer({self.data}), start_timestamp={self.start_timestamp}, end_timestamp={self.end_timestamp})"


class DataProcessor:

    def __init__(self, queue_name):
        self.sqs_client = boto3.client('sqs')
        self.queue_url = self.sqs_client.get_queue_url(QueueName=queue_name)['QueueUrl']
        db_host = os.environ['DB_HOST']
        db_user = os.environ['DB_USER']
        db_password = os.environ['DB_PASSWORD']
        db_database = os.environ['DB_DATABASE']
        self.engine = create_engine(f"mysql+mysqlconnector://{db_user}:{db_password}@{db_host}/{db_database}")
        Base.metadata.create_all(bind=self.engine)
        self.session = sessionmaker(bind=self.engine)()

    def getMessageFromSQS(self):
        response = self.sqs_client.receive_message(QueueUrl=self.queue_url, MaxNumberOfMessages=1)
        if 'Messages' in response:
            message = response['Messages'][0]
            return message
        else:
            return None

    def queryDB(self, key):
        item = self.session.query(DataItem).filter_by(channel_id=key).first()
        if item is not None:
            return {'Id': item.id, 'ChannelId': item.channel_id, 'Data': numpy.frombuffer(item.data), 'StartTimestamp': item.start_timestamp, 'EndTimestamp': item.end_timestamp}
        else:
            return None

    def process(self, message):
        data = message['Body']
        # process the data here
        processed_data = numpy.array(data) # for example
        start_timestamp = int(message['MessageAttributes']['StartTimestamp']['StringValue'])
        end_timestamp = int(message['MessageAttributes']['EndTimestamp']['StringValue'])
        channel_id = message['MessageAttributes']['ChannelId']['StringValue']
        return {'ChannelId': channel_id, 'Data': processed_data, 'StartTimestamp': start_timestamp, 'EndTimestamp': end_timestamp}

    def writeDB(self, item):
        data_item = DataItem(channel_id=item['channel_id'],
                         data=item['Data'].tobytes(),
                         start_timestamp=item['StartTimestamp'],
                         end_timestamp=item['EndTimestamp'])
        self.session.add(data_item)
        self.session.commit()
        return {'Id': data_item.id, 'ChannelId': data_item.channel_id, 'Data': numpy.frombuffer(data_item.data), 'StartTimestamp': data_item.start_timestamp, 'EndTimestamp': data_item.end_timestamp}

    def run(self):
        message = self.getMessageFromSQS()
        if message is not None:
            key = message['MessageAttributes']['ChannelId']['StringValue']
            existing_data = self.queryDB(key)
            if existing_data is not None:
                data = numpy.concatenate((existing_data['Data'], self.process(message)['Data']))
                start_timestamp = existing_data['StartTimestamp']
                end_timestamp = self.process(message)['EndTimestamp']
                item = {'ChannelId': key, 'Data': data, 'StartTimestamp': start_timestamp, 'EndTimestamp': end_timestamp}
            else:
                item = self.process(message)
            self.writeDB(item)

if __name__ == '__main__':
    sqs_queue_name = os.environ['SQS_QUEUE_NAME']
    DP = DataProcessor(queue_name=sqs_queue_name)
    DP.run()