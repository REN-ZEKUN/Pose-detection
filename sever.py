import socket
import time
import pygame

HOST = '192.168.137.24' #连接本地服务器，可通过ipconfig/all看IPV4的地址,ip of raspberry
PORT = 8001 #设置端口号，自己设置即可
pygame.mixer.init()
# socket.AF_INET用于服务器与服务器之间的网络通信
# socket.SOCK_STREAM代表基于TCP的流式socket通信
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind((HOST, PORT)) #绑定端口，告诉别人，这个端口我使用了，其他人别用了
sock.listen(5)  #监听这个端口，可连接最多5个设备
while True:  #死循环，服务器端一直提供服务。
    connection,address = sock.accept() #接收客户端的连接请求
    try:
        connection.settimeout(10)  #设置10s时限
        buf = connection.recv(1024)#接收数据实例化
        if buf:  #接收成功
            connection.send(b'welcome to server!')  #发送消息，b表示bytes类型
            buf=buf.decode('utf-8')
            flag=buf[-4:-2]
            print(buf)#打印接收到的消息看看
            print(buf[-4:-2])
            if flag=='11':
                pygame.mixer.music.load('body_head_front.mp3')
                pygame.mixer.music.play()
            elif flag=='12':
                pygame.mixer.music.load('body_frt,head_bk.mp3')
                pygame.mixer.music.play()    
            elif flag=='21':
                pygame.mixer.music.load('body_bk,head_frt.mp3')
                pygame.mixer.music.play() 
            elif flag=='22':
                pygame.mixer.music.load('body_head_back.mp3')
                pygame.mixer.music.play()
            elif flag=='10':
                pygame.mixer.music.load('body_front.mp3')
                pygame.mixer.music.play()
            elif flag=='01':
                pygame.mixer.music.load('head_front.mp3')
                pygame.mixer.music.play()
            elif flag=='20':
                pygame.mixer.music.load('body_back.mp3')
                pygame.mixer.music.play()
            elif flag=='02':
                pygame.mixer.music.load('head_back.mp3')
                pygame.mixer.music.play()
        else:  #接收失败
            connection.send(b'Please go out!')
    except socket.timeout:  #超时
        print('time out!')
    connection.close()  #关闭连接