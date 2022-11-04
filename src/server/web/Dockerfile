FROM python:alpine

WORKDIR /app
COPY requirements.txt requirements.txt
RUN pip3 install -r requirements.txt

COPY ./app.py ./app.py
COPY ./templates/ ./templates/
COPY ./static ./static

CMD ["python3", "-m" , "flask", "run", "--host=0.0.0.0"]