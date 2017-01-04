from pymongo import MongoClient

client = MongoClient('192.168.56.101', 27017)
db = client['bookmarks_db']
collection = db['bookmarks_collection']

result = collection.delete_many({})
deleted=result.deleted_count
print("deleted: {0}".format(deleted))