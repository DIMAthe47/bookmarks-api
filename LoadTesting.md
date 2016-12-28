Configuration for fastcgi: threads="2" queue="1000"

###LoadTesting:
####line(1,5000,1m) - https://overload.yandex.net/5451#tab=test_data&tags=&plot_groups=main&machines=&metrics=&slider_start=1482888749&slider_end=1482888815
На графиках видно, что время ответа начинает расти с ~2300rps, ошибки начинают появляться с ~3000rps.
Поэтому можно предполагать, что при 2500rps будет стабильная работа.
####line(1,2500,30s) const(2500,5m) -https://overload.yandex.net/5457#tab=test_data&tags=&plot_groups=main&machines=&metrics=&slider_start=1482892406&slider_end=1482892734

Интересно, что даже если мы понизим до 2000rps, результаты будут почти такими же
####line(1,2000,30s) const(2000,5m) - https://overload.yandex.net/5456#tab=test_data&tags=&plot_groups=main&machines=&metrics=&slider_start=1482890588&slider_end=1482890916
