# Отчет о проделанной работе

## Цель работы
Основной целью данного проекта было развертывание системы непрерывной интеграции и развертывания (CI/CD) с использованием GitLab Runner. В ходе выполнения были настроены автоматические сборки, тестирование кодстайла, интеграционные тесты, а также деплой на отдельный сервер.

---

### Part 1. Настройка **gitlab-runner**

1. Поднял виртуальную машину *Ubuntu Server 22.04 LTS*.
![ubuntu_server](./screenshots/pic.png)
2. Скачал и установил на виртуальную машину **gitlab-runner**.
![gitlab_runner](./screenshots/pic1.png)
3. Для регистрации понадобятся URL и токен, которые можно получить на страничке задания на платформе.
![gitlab_runner_token](./screenshots/pic2.png)
4. Запустил **gitlab-runner** и зарегистрировал его для использования в текущем проекте (*DO6_CICD*).
![gitlab_runner_start](./screenshots/pic3.png)
![gitlab_runner_token](./screenshots/pic4.png)

### Part 2. Сборка

1. Написал этап для **CI** по сборке приложений из проекта *C2_SimpleBashUtils*.
2. В файле _gitlab-ci.yml_ добавил этап запуска сборки через мейк файл из проекта _C2_.
3. Файлы, полученные после сборки (артефакты), сохрани в произвольную директорию со сроком хранения 30 дней.
![gitlab_build](./screenshots/pic5.png)
4. Результат работы pipeline 
![gitlab_build_result](./screenshots/pic6.png)
    - Проект успешно забилдился.
    - Исполняемые файлы были сохранены на 30 дней.
### Part 3. Тест кодстайла

1. Написал этап для **CI**, который запускает скрипт кодстайла (*clang-format*).
![gitlab_code_style](./screenshots/pic7.png)
2. В пайплайне отобразил вывод утилиты *clang-format*, если пайплан прошел.
![gitlab_code_style1](./screenshots/pic8.png)
![gitlab_code_style2](./screenshots/pic9.png)
3. Если кодстайл не прошел, то нужно «зафейлить» пайплайн.
![gitlab_code_style3](./screenshots/pic10.png)
![gitlab_code_style4](./screenshots/pic11.png)
### Part 4. Интеграционные тесты

1. Напиcал этап для **CI**, который запускает интеграционные тесты из того же проекта.
![gitlab_project_test](./screenshots/pic15.png)
2. Запустил этот этап автоматически только при условии, если сборка и тест кодстайла прошли успешно.
![gitlab_project_test](./screenshots/pic16.png)
![gitlab_project_test](./screenshots/pic17.png)
![gitlab_project_test](./screenshots/pic18.png)
![gitlab_project_test](./screenshots/pic19.png)
3. Также проверил,что автоматический пропускается,если сборка или тест кодстайл зафейлились.
 ![gitlab_project_test](./screenshots/pic12.png)
 ![gitlab_project_test](./screenshots/pic13.png)
 ![gitlab_project_test](./screenshots/pic14.png)
4. Если тесты не прошли, то пайплайн должен зафейлить.
![gitlab_project_test](./screenshots/pic20.png)
![gitlab_project_test](./screenshots/pic21.png)
![gitlab_project_test](./screenshots/pic22.png)
![gitlab_project_test](./screenshots/pic23.png)

### Part 5. Этап деплоя

1. Поднял вторую виртуальную машину *Ubuntu Server 22.04 LTS*.

2. Написал этап для **CD**, который «разворачивает» проект на другой виртуальной машине. Этап запускается вручную при условии, что все предыдущие этапы прошли успешно.
![gitlab_deploy](./screenshots/pic24.png)
3. Написал bash-скрипт, который при помощи **ssh** и **scp** копирует файлы, полученные после сборки (артефакты), в директорию */usr/local/bin* второй виртуальной машины и в файле _gitlab-ci.yml_ добавил этап запуска написанного скрипта.
![gitlab_deploy](./screenshots/pic25.png)
4. Провел статическую маршрутизацию между двумя машинами и проверил соединение между машинами.
![gitlab_deploy](./screenshots/pic26.png)
![gitlab_deploy](./screenshots/pic27.png)
![gitlab_deploy](./screenshots/pic28.png)
![gitlab_deploy](./screenshots/pic29.png)
5. Сгенерировал ssh-ключ на раннере и скопировал ssh-ключ на вторую машину.
![gitlab_deploy](./screenshots/pic30.png)
![gitlab_deploy](./screenshots/pic31.png)
6. Перезапустил службу раннера командой `sudo systemctl restart gitlab-runner.service`
7. В результате получил готовые к работе приложения из проекта *C2_SimpleBashUtils* (s21_cat и s21_grep) на второй виртуальной машине.
![gitlab_deploy](./screenshots/pic32.png)

---

## Вывод
В результате выполнения проекта я:
- Освоил установку и настройку GitLab Runner.
- Настроил автоматическую сборку, тестирование и деплой проекта.
- Автоматизировал процесс контроля кодстайла и интеграционного тестирования.
- Организовал развертывание артефактов на удаленный сервер.
- Настроил уведомления о статусе пайплайна в Telegram.

Проект позволил получить практический опыт работы с CI/CD в GitLab, что является важным навыком для DevOps-инженера и системного администратора.