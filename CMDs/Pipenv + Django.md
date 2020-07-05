# Pipenv + Django

## 建立虚拟环境

```shell
pipenv --python 3.7 
pipenv install django
pipenv --venv # 当前虚拟环境的位置
pipenv shell  # 进入虚拟环境
exit          # 退出虚拟环境
```

## 建立项目

```shell
django-admin startproject ${NAME}
cd ${NAME}
python manage.py startapp ${APP_NAME}   # 注意不要重名
python manage.py migrate
python manage.py createsuperuser
python manage.py runserver ${端口号}
```

