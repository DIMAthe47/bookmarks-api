Configuration for fastcgi: threads="2" queue="1000"

###LoadTesting:
####line(1,5000,1m) - https://overload.yandex.net/5451#tab=test_data&tags=&plot_groups=main&machines=&metrics=&slider_start=1482888749&slider_end=1482888815
На графиках видно, что время ответа начинает расти с ~2300rps, ошибки начинают появляться с ~3000rps.
####line(1,2000,30s) const(2000,5m) - https://overload.yandex.net/5456#tab=test_data&tags=&plot_groups=main&machines=&metrics=&slider_start=1482890588&slider_end=1482890916
