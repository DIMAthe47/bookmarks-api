from pymongo import MongoClient


"""
bookmark = {"url": "kp.by" }
inserted_id=collection.insert_one(bookmark).inserted_id
print(inserted_id)
"""

def fetch_all_str_ids(collection):
    str_ids=[]
    for bookmark in collection.find():
        str_id=str(bookmark["_id"])
        str_ids.append(str_id)
        #print(str_id)
    return str_ids

def fetch_all_docs(collection):
    for bookmark in collection.find():
        print(bookmark)

if __name__ == '__main__':
    client = MongoClient('192.168.56.101', 27017)
    db = client['bookmarks_db']
    collection = db['bookmarks_collection']

    #fetch_all_str_ids(collection)
    fetch_all_docs(collection)
    #print(collection.count())