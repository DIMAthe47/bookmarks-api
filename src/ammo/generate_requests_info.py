import random
import string
import numpy as np
from pymongo import MongoClient
import fetch_ids
import json
from common import random_word
from bson.objectid import ObjectId

requests_names = ["read_bookmark", "create_bookmark", "update_bookmark", "delete_bookmark"]
requests_probabilities = [0.8,0.15,0.01,0.04]
#requests_probabilities = [1,0,0,0]
requests_count = 10 ** 4

client = MongoClient('192.168.56.101', 27017)
db = client['bookmarks_db']
collection = db['bookmarks_collection']

max_url_length=20
max_description_length = 40

def main():
    each_num_request_update_ids=100

    for i in xrange(requests_count):
        if i%each_num_request_update_ids==0:
            bookmarks_str_ids = fetch_ids.fetch_all_str_ids(collection)

        req = np.random.choice(np.array(requests_names), 1, p=requests_probabilities)

        if req == "read_bookmark":
            rand_bookmark_num = random.randint(0, len(bookmarks_str_ids)-1)
            bookmark_id=bookmarks_str_ids[rand_bookmark_num]

            request_path='/bookmarks/'+bookmark_id

            request_info="{}||{}||{}".format('GET', request_path, "read_bookmark")
            print request_info
        elif req=="create_bookmark":
            url = random_word(max_url_length) + ".com"
            description = random_word(max_description_length)
            body=json.dumps({"url":url,"description":description})
            request_path = '/bookmarks/'
            request_info = "{}||{}||{}||{}".format('POST', request_path, "post_bookmark",body)
            print request_info
        elif req=="update_bookmark":
            rand_bookmark_num = random.randint(0, len(bookmarks_str_ids) - 1)
            bookmark_id = bookmarks_str_ids[rand_bookmark_num]

            oid = ObjectId(bookmark_id)
            bookmark=collection.find_one({'_id': oid})

            description=random_word(max_description_length)
            body = json.dumps({"_id":{"$oid":str(oid)}, "url":bookmark["url"] , "description":description})

            request_path = '/bookmarks/' + bookmark_id
            request_info = "{}||{}||{}||{}".format('PUT', request_path, "update_bookmark", body)
            print request_info
        elif req == "delete_bookmark":
            rand_bookmark_num = random.randint(0, len(bookmarks_str_ids) - 1)
            bookmark_id = bookmarks_str_ids[rand_bookmark_num]
            bookmarks_str_ids.remove(bookmark_id)

            #body = json.dumps({"_id": {"$oid": bookmark_id}})

            request_path = '/bookmarks/' + bookmark_id
            request_info = "{}||{}||{}".format('DELETE', request_path, "delete_bookmark")
            print request_info

if __name__ == "__main__":
    main()