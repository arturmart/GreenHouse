## Шаг 1: Подготовка программы
Убедитесь, что ваша программа (main) находится в директории /home/orangepi/greenhouse/Demo и имеет права на выполнение.

Проверьте, что файл существует:

```bash
ls -l /home/orangepi/greenhouse/Demo/main
```
Вы должны увидеть что-то вроде:

```bash
-rwxr-xr-x 1 orangepi orangepi 12345 Dec 22 12:00 /home/orangepi/greenhouse/Demo/main
```
Если программа не имеет прав на выполнение, задайте их:

```bash
chmod +x /home/orangepi/greenhouse/Demo/main
```
--------
## Шаг 2: Создание файла службы
Создайте файл службы systemd:

Откройте редактор nano для создания файла:

```bash

sudo nano /etc/systemd/system/greenhouse-demo.service
```
Вставьте следующее содержимое:

```ini
[Unit]
Description=Greenhouse Demo Program
After=network.target

[Service]
WorkingDirectory=/home/orangepi/greenhouse/Demo
ExecStart=/home/orangepi/greenhouse/Demo/main
Restart=always
User=root
Environment="CONFIG_PATH=/home/orangepi/greenhouse/config.json" "DEBUG=true"

[Install]
WantedBy=multi-user.target
Объяснение:
WorkingDirectory: Указывает каталог, из которого будет запускаться программа.
ExecStart: Путь к вашей программе.
Restart=always: Автоматический перезапуск программы, если она завершилась с ошибкой.
User=root: Запуск службы с правами root, чтобы не требовался sudo.
Environment: Задаёт переменные окружения (например, путь к конфигурации и отладочный режим).
Сохраните файл (Ctrl+O, затем Enter, и Ctrl+X для выхода).
```
-------
## Шаг 3: Применение изменений в systemd
Перезагрузите менеджер systemd, чтобы он узнал о новой службе:

```bush
sudo systemctl daemon-reload
```

Активируйте автозапуск службы при загрузке системы:

```bash
sudo systemctl enable greenhouse-demo.service
```

------
## Шаг 4: Запуск и проверка
Запустите службу:

```bash
sudo systemctl start greenhouse-demo.service
```

Проверьте состояние службы:

```bash
sudo systemctl status greenhouse-demo.service
```

Вы должны увидеть что-то вроде:

```yaml

● greenhouse-demo.service - Greenhouse Demo Program
     Loaded: loaded (/etc/systemd/system/greenhouse-demo.service; enabled)
     Active: active (running) since Thu 2024-12-22 12:34:56 UTC
```

-----

## Шаг 5: Проверка автозапуска
Перезагрузите систему, чтобы убедиться, что служба запускается автоматически:

```bash
sudo reboot
```

После перезагрузки проверьте статус службы:

```bash
sudo systemctl status greenhouse-demo.service
```

-----

## Шаг 6: Просмотр логов
Если нужно проверить, как работает ваша программа, используйте команду для просмотра логов:

```bash
journalctl -u greenhouse-demo.service
```

Дополнительно: Запуск от пользователя orangepi без запроса пароля
Если вы хотите запускать программу от имени пользователя orangepi (не root), но с привилегиями sudo, сделайте следующее:

Откройте файл конфигурации sudo:

```bash
sudo visudo
```

Добавьте строку в конец файла:

```plaintext
orangepi ALL=(ALL) NOPASSWD: /home/orangepi/greenhouse/Demo/main
```

Измените файл службы:

```bash
sudo nano /etc/systemd/system/greenhouse-demo.service
```

Замените строку User=root на:

```ini
User=orangepi
ExecStart=sudo /home/orangepi/greenhouse/Demo/main
```

Сохраните изменения и перезагрузите systemd:

```bash
sudo systemctl daemon-reload
```

Теперь пользователь orangepi сможет запускать программу без запроса пароля.
-----
Итог
Теперь ваша программа настроена на запуск через systemd, она автоматически стартует при включении системы, использует правильный рабочий каталог, и пароль для её запуска не требуется.
-----
## Остановить службу
Чтобы остановить запущенную службу:

```bash
sudo systemctl stop greenhouse-demo.service
```
## Запустить службу
Если служба остановлена, запустите её снова:

```bash

sudo systemctl start greenhouse-demo.service
```
## Перезапустить службу
Перезапуск службы остановит её и сразу же запустит заново. Это полезно после изменений в программе или конфигурации:

```bash
sudo systemctl restart greenhouse-demo.service
```
## Проверить статус службы
Чтобы проверить, работает ли служба и нет ли ошибок:

```bash
sudo systemctl status greenhouse-demo.service
```

Пример вывода:

```yaml

● greenhouse-demo.service - Greenhouse Demo Program
     Loaded: loaded (/etc/systemd/system/greenhouse-demo.service; enabled)
     Active: active (running) since Thu 2024-12-22 12:34:56 UTC
   Main PID: 1234 (main)
      Tasks: 1 (limit: 1010)
     Memory: 2.3M
     CGroup: /system.slice/greenhouse-demo.service
             └─1234 /home/orangepi/greenhouse/Demo/main
```
## Просмотреть логи службы
Чтобы увидеть логи службы и найти возможные ошибки:

```bash
journalctl -u greenhouse-demo.service
journalctl -u greenhouse-demo.service > output.txt
```

Если вы хотите видеть только последние строки и автоматически обновлять их в реальном времени, используйте:

```bash
journalctl -u greenhouse-demo.service -f
```
## Полезные команды:
Отключить автозапуск службы:

```bash
sudo systemctl disable greenhouse-demo.service
```
## Включить автозапуск службы:

```bash
sudo systemctl enable greenhouse-demo.service
```
## Удалить службу (если больше не нужна):

```bash
sudo systemctl stop greenhouse-demo.service
sudo systemctl disable greenhouse-demo.service
sudo rm /etc/systemd/system/greenhouse-demo.service
sudo systemctl daemon-reload
```
Теперь вы знаете, как управлять службой systemd! 🚀
