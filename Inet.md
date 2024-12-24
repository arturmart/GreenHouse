## 1. Использование nmcli (Network Manager Command Line Interface):
Эта утилита работает, если у вас установлен Network Manager.

Просмотр доступных Wi-Fi сетей:

```bash
nmcli dev wifi list
```
Подключение к сети Wi-Fi:

```bash
nmcli dev wifi connect "SSID_сети" password "ваш_пароль"
```
--------
## 2. Проверка статуса сети:
Проверить текущее подключение к Wi-Fi:

```bash
nmcli connection show
```

```bash
ip a
```
Если Wi-Fi адаптер не работает, проверьте драйверы или убедитесь, что устройство поддерживает Wi-Fi. Если нужна помощь с подключением или настройкой, напишите!
