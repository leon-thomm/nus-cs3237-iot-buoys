FROM python:alpine

WORKDIR /usr/src/listener
COPY requirements.txt ./
COPY mqtt-listener.py ./
RUN pip3 install -r requirements.txt

CMD ["python3", "./mqtt-listener.py"]