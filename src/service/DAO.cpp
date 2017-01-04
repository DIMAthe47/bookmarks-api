#include "DAO.h"

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/stdx.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

const std::string DB_NAME = "bookmarks_db";
const std::string BOOKMARKS_COLLECTION_NAME = "bookmarks_collection";

boost::thread_specific_ptr<mongocxx::client> DAO::clientPtr;

void DAO::resetClientConnection() {
        clientPtr.reset(new mongocxx::client(mongocxx::uri{}));
}

mongocxx::client& DAO::getClient() {
        if (!clientPtr.get())
            resetClientConnection();
        return *clientPtr.get();
}

void setPrimaryPreferredReadMode(mongocxx::client& client) {
    mongocxx::read_preference primary_preferred{mongocxx::read_preference::read_mode::k_primary_preferred};
    client.read_preference(primary_preferred);
}

std::string DAO::readBookmark(const std::string& bookmarkId) {
    //std::cout<<"READ "<<bookmarkId<<std::endl;
    auto& conn = getClient();
    setPrimaryPreferredReadMode(conn);

    auto collection = conn[DB_NAME][BOOKMARKS_COLLECTION_NAME];

    mongocxx::stdx::optional<bsoncxx::document::value> maybe_result = 
        collection.find_one(document{}<<"_id"<<bsoncxx::oid(bookmarkId)<<finalize);
    if (maybe_result) {
        auto view = maybe_result->view();
        std::string bookmarkJson=bsoncxx::to_json(view);
        //std::cout<<bookmarkJson<<std::endl;
        return bookmarkJson;
    }else{
        return std::string();
    }
}

void DAO::updateBookmark(const std::string& bookmarkJson) {
    auto& conn = getClient();
    auto collection = conn[DB_NAME][BOOKMARKS_COLLECTION_NAME];
    //std::cout<<bookmarkJson<<std::endl;

    //TODO check format
    bsoncxx::document::value bookmarkBson=bsoncxx::from_json(bookmarkJson);

    bsoncxx::types::b_oid bookmarkId=bookmarkBson.view()["_id"].get_oid();
    mongocxx::stdx::optional<mongocxx::result::update> maybe_update = 
        collection.update_one(document{}<<"_id"<<bookmarkId<<finalize,document{}<<"$set"<<bookmarkBson.view()<<finalize);
   
    //TODO check if succeed. getLastErrorObj() 
}

std::string DAO::createBookmark(const std::string& bookmarkJson) {
    // std::cout<<"CREATE "<<bookmarkJson<<std::endl;
    auto& conn = getClient();
    auto collection = conn[DB_NAME][BOOKMARKS_COLLECTION_NAME];
   // std::cout<<bookmarkJson<<std::endl;

    bsoncxx::document::value bookmarkBson=bsoncxx::from_json(bookmarkJson);
    mongocxx::stdx::optional<mongocxx::result::insert_one> insert_one_result=collection.insert_one(bookmarkBson.view());
    bsoncxx::types::value inserted_id=insert_one_result.value().inserted_id();

    mongocxx::stdx::optional<bsoncxx::document::value> maybe_result = 
        collection.find_one(document{}<<"_id"<<inserted_id<<finalize);
    if (maybe_result) {
        auto view = maybe_result->view();
        std::string bookmarkJson=bsoncxx::to_json(view);
        //std::cout<<bookmarkJson<<std::endl;
        return bookmarkJson;
    }else{
        return std::string();
    }

    //TODO check if succeed. getLastErrorObj()
}

void DAO::deleteBookmark(const std::string& bookmarkId){
    //std::cout<<"DELETE "<<bookmarkId<<std::endl;

    auto& conn = getClient();
    auto collection = conn[DB_NAME][BOOKMARKS_COLLECTION_NAME];
    mongocxx::stdx::optional<mongocxx::result::delete_result> maybe_delete = 
        collection.delete_one(document{}<<"_id"<<bsoncxx::oid(bookmarkId)<<finalize);

    //TODO check if succeed. getLastErrorObj() 
}