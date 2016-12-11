# bookmarks-api
bookmarks rest api

fastcgi
1. replace /etc/nginx/sites-enabled/default with default
1. build so from cpp (g++ SimpleClass.cpp -fPIC -lfastcgi-daemon2 -shared -o libcsimple.so)
2. reference this so in csimple.conf
3. run 'fastcgi-daemon2 --config=csimple.conf'
4. run 'curl -v "http://localhost/simple_res"' - must see Hello world! at the end of response body
