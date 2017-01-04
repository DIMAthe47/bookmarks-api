
Сразу нужно сказать, что с реализацией появились проблемы. 
Так api предполагал вложенность bookmarkCollection от "родителя к детям" и делать путь вида
bookmarksCollections/{bookmarkCollectionId}/bookmarks/{bookmarkId}, но когда дело дошло до монги, стало понтяно, что 
если вложенность и реализовывать, то нужно делать зависимость наоборот: ребёнок->родитель. И 
вообще, предполагалось, что точка входа - bookmarkCollections/ - которая выдаст клиенту все доступные ему коллекции.
То есть здесь есть увязка пользователя с коллекциями, а реализовывать её не хотелось. 
Откровенно говоря, с bookmarkCollection и вложенностью нужно было бы пересмотреть кардинально весь api.
Ну а для простоты и хоть какого-то сервиса, будем предполагать, что пользователь как-то получил коллекцию и в ней видит links, ведущие на конкретные bookmarks, будем работать только с "bookmarks/"

##REST
Самые спорные вопросы, касающиеся REST - hypermedia, self descriptive messages.
####Hypermedia.
Хотелось реализовать, но появились проблемы с самим api(логикой, организацией), с реализацией. Поэтому, можно сказать,
было не до "hypermedia".
####Self descriptive messages
Ещё менее важным мне кажется self descriptive messages(по крайней мере для нашего сервиса). Описывать типы можно было бы когда всё
уже вылизано, и осталось только дошлифовать сервис, чтоб считаться истинным REST.

То есть мы имеем не совсем REST сервис.


##CAP
Сервис использует mongodb.
* Чтобы усилить availability системы(а самой частой операцией у нас является чтение закладки)для чтения используется readMode: primaryPreffered.
* Eventual Consistency обеспечиваться монгой по умолчанию.

##Load Testing
Тестируя HelloWorld, можно было сказать, что наш сервис выдерживал ~2000-2500rps.
При добавление mongo, максимальный примелемый rps понизился.


 
line(1,2500,60s)(только GET) - https://overload.yandex.net/6158#tab=test_data&tags=&plot_groups=main&machines=&metrics=&slider_start=1483515379&slider_end=1483515437

Видно, что 2500 сервис не выдерживает, поэтому попробвал для 2000.

line(1,2000,30s) const(2000,5m)(только GET) - https://overload.yandex.net/6160#tab=test_data&tags=&plot_groups=main&machines=&metrics=&slider_start=1483515644&slider_end=1483515972

Но и 2000 получислось неуверенно. Попробвал на 1500.

line(1,1500,30s) const(1500,5m)(только GET) - https://overload.yandex.net/6161#tab=test_data&tags=a32be760159f283e858809e9b1a7edc2&plot_groups=main&machines=8&metrics=&slider_start=1483516061&slider_end=1483516389&compress_ratio=1

Здесь ошибок уже почти не видно, но бэйдж "net" получился всё-равно красным.

Всё это тесты были только на GET, при этом даже на 1500 "net" не зачтён.

Для всевозможных операций GET,POST,PUT,DELETE генерировался ammo.txt исходя из имеющихся данных в базе -
перед тестом в базу заносилось 1000 записей, при этом относительная частота была расставлена так:
* read_bookmark - 0.8
* create_bookmark - 0.15
* update_bookmark - 0.01
* delete_bookmark - 0.04

Чтобы был запас прочности и всё-таки был зачтён "net", rps выставлялся в 1000.
line(1,1000,30s) const(1000,5m) - https://overload.yandex.net/6162#tab=test_data&tags=&plot_groups=main&machines=&metrics=&slider_start=1483517540&slider_end=1483517868&compress_ratio=1



