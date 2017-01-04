import random
import string
import numpy as np
from pymongo import MongoClient
import fetch_ids
import json
from common import random_word

def create_bookmark(collection):
    url = random_word(20) + ".com"
    description = random_word(40)
    bookmark = {"url": url, "description": description}
    inserted_id = collection.insert_one(bookmark).inserted_id
    return inserted_id

def pregenerte_bookmarks(count,collection):
    for i in range(count):
        inserted_id=create_bookmark(collection)
        inserted_str_id=str(inserted_id)
        print(inserted_str_id)

def main():
    client = MongoClient('192.168.56.101', 27017)
    db = client['bookmarks_db']
    collection = db['bookmarks_collection']

    bookmarks_to_create_count=1000
    bookmarks_str_ids=[]
    pregenerte_bookmarks(bookmarks_to_create_count,collection)


if __name__ == "__main__":
    main()


