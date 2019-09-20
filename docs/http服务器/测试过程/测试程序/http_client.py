
import requests
import threading
import random

exitFlag = 0
import time
class myThread (threading.Thread):
    def __init__(self, threadID, name, counter):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
        self.counter = counter
    def run(self):
        i = 0
        s = requests.Session()
        n = 0
        while True:
            if n == 0:
                n += 1
                if self.counter >= 1 and self.counter <= 11:
                    continue
                sendWithNoTimeout(s)
                time.sleep(30)
                continue

            i += 1
            print("第 %d 个线程，发送第 %d 次" %(self.counter, i))
            try:
                if self.counter >= 1 and self.counter <= 11:
                    send()     # 短链接
                else:
                    sendKeepalive(s)     # 长连接
            except Exception as e:
                print(e)

            # break
            time.sleep(random.randint(5, 10))
            # time.sleep(20)
def sendWithNoTimeout(s):
    url = "http://192.168.12.250:3000/file_test/1.png"
    # url = "http://192.168.12.250:3000/html/index.html"

    header = {
        'Connection': 'keep-alive'
    }
    response = s.request("GET", url, headers=header, timeout=10000)
    print(response.status_code)

def sendKeepalive(s):
    url = "http://192.168.12.250:3000/file_test/1.png"
    # url = "http://192.168.12.250:3000/html/index.html"

    header = {
        'Connection': 'keep-alive'
    }
    response = s.request("GET", url, headers=header, timeout=random.randint(0, 30))
    print(response.status_code)

def send():
    url = "http://192.168.12.250:3000/file_test/1.png"
    # url = "http://192.168.12.250:3000/html/index.html"

    header = {
        'Connection': 'keep-alive'
    }
    response = requests.request("GET", url, headers=header, timeout=random.randint(0, 30))
    print(response.status_code)

if __name__ == '__main__':
    # 创建新线程
    thread_list = []

    for i in range(0, 5000):
        thread = myThread(1, "Thread", i)
        thread_list.append(thread)
    for t in thread_list:
        t.start()

    for t in thread_list:
        t.join()