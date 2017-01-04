#pragma once

#include <boost/thread/tss.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include <string>

class DAO {

private:

    static boost::thread_specific_ptr<mongocxx::client> clientPtr;
    static void resetClientConnection();
    static mongocxx::client& getClient();  

public:

    DAO() {
        mongocxx::instance inst{};
    }

    std::string createBookmark(const std::string& bookmarkJson);
    std::string readBookmark(const std::string& bookmarkId);
    void updateBookmark(const std::string& bookmarkJson);
    void deleteBookmark(const std::string& bookmarkId);
    

};
