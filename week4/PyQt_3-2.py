from pymongo import MongoClient  # pip install pymongo, pip install dnspython

client = MongoClient("<mongodb url>")

db = client['<데이터베이스 이름>']

for d, cnt in zip(db['sensors'].find(), range(10)):
    print(d['pm1'], d['pm2'], d['pm10'])
