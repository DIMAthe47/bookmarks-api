#include "DAO.h"

#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>
#include <fastcgi2/stream.h>

#include <exception>
#include <iostream>
#include <string>
#include <sstream>

class BookmarksHandler : virtual public fastcgi::Component, virtual public fastcgi::Handler {

public:
    BookmarksHandler(fastcgi::ComponentContext *context) :
        fastcgi::Component(context) {
    }

    virtual ~BookmarksHandler() {
    }

    virtual void onLoad() {
    }

    virtual void onUnload() {
    }

    virtual void handleRequest(fastcgi::Request *request, fastcgi::HandlerContext *context) {
        try {
            const std::string method = request->getRequestMethod();
            if (method == "GET") {
                handleGET(request, context);
            }
            else if (method == "POST") {
                handlePOST(request, context);
            }
            else if (method == "PUT") {
                handlePUT(request, context);
            }
            else if (method == "DELETE") {
                handleDELETE(request, context);
            }
            else {
                request->setStatus(405);
            }
           
        }
        catch (const std::string& s) {
            request->setContentType("text/plain");
            fastcgi::RequestStream stream(request);
            stream << s << std::endl;
            request->setStatus(500);
        }
    }

private:

    DAO dao;

    std::string extractIdFromRequestPath(fastcgi::Request* request) const {
        const std::string prefix = "/bookmarks/";
        const std::string request_path = request->getScriptName();
        //std::cout<<request_path<<std::endl;
        if (request_path.size() <= prefix.size()) {
            return std::string();
        } else {
            std::string id=request_path.substr(prefix.size());
            return id;
        }
    }

    void handlePOST(fastcgi::Request *request, fastcgi::HandlerContext *context) {
        fastcgi::DataBuffer bodyBuffer = request->requestBody();
        //TODO check if valid format
        std::string bookmarkJson;
        bodyBuffer.toString(bookmarkJson);
        std::string createdBookmarkJson=dao.createBookmark(bookmarkJson);
        
        const char* c_str=createdBookmarkJson.c_str();
        request->write(c_str,strlen(c_str));
        request->setStatus(201);
    }

    void handleGET(fastcgi::Request *request, fastcgi::HandlerContext *context) {
        std::string id = extractIdFromRequestPath(request);
        if(id.size()!=0){
            std::string bookmarkJson=dao.readBookmark(id);
            if(bookmarkJson.size()==0){
                 request->setContentType("text/plain");
                 const char* not_found_text="404 Not Found";
                 request->write(not_found_text,strlen(not_found_text));
                 request->setStatus(404);
            }else{
                request->setContentType("application/json");
                const char* c_str=bookmarkJson.c_str();
                request->write(c_str,strlen(c_str));
                request->setStatus(200);
            }
        }else{
            request->setContentType("text/plain");
            const char* not_allowed_text="405 Method Not Allowed";
            request->write(not_allowed_text,strlen(not_allowed_text));
            request->setStatus(405);
        }
    }

    void handlePUT(fastcgi::Request *request, fastcgi::HandlerContext *context) {
        std::string id = extractIdFromRequestPath(request);

        std::string bookmarkJson;
        fastcgi::DataBuffer bodyBuffer = request->requestBody();
        //TODO check if valid format
        bodyBuffer.toString(bookmarkJson);
        dao.updateBookmark(bookmarkJson);
        request->setStatus(204);
    }

    void handleDELETE(fastcgi::Request *request, fastcgi::HandlerContext *context) {
         std::string id = extractIdFromRequestPath(request);
         dao.deleteBookmark(id);
         request->setStatus(204);

        /*
        std::string id = getId(request);
        db.DeletePost(id);
        */
    }
};

FCGIDAEMON_REGISTER_FACTORIES_BEGIN()
FCGIDAEMON_ADD_DEFAULT_FACTORY("bookmarksHandler_factory", BookmarksHandler)
FCGIDAEMON_REGISTER_FACTORIES_END()