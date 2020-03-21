import socket
import threading
import time
from PyQt5 import QtCore, QtGui, QtWidgets
import pyqtgraph as pg
import sys
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
import pyperclip
import qtawesome
import cv2
import win32api
import HKIPcamera
import numpy as np
import datetime
import os

from multiprocessing import Process, Queue,Value
import multiprocessing as mp
BUFSIZE = 1024
DIM = (1280, 720)
K = np.array([[911.1546799130407, 0.0, 636.1179417117924], [0.0, 909.46867680695, 362.22498374331553], [0.0, 0.0, 1.0]])
D = np.array([[-0.20518436786315236], [0.13086850373804892], [-0.38920896106289166], [0.3717694153970601]])
DIM2 = (640, 450)
K2 = np.array(
    [[454.6079430869957, 0.0, 318.3465274330906], [0.0, 566.2354983838005, 226.9368868881296], [0.0, 0.0, 1.0]])
D2 = np.array([[-0.20234958495037134], [0.1432242196055454], [-0.4347283951611514], [0.4076209075432136]])


class MainUi(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self.init_ui()
        self.data1_list = []
        self.data2_list = []
        self.time_list = []
        self.time2_list = []
        self.orginrec_list = []
        self.start_state = Value('d',0)
        self.stop_state = 0
        self.copy_state = 0
        self.data_type = 0
        self.thread_flag = Value('d',0)
        self.dataQueue=Queue()
        self.dataoutQueue=Queue()
        self.printprogress = 0
        self.printprogress2 = 0
        self.stop2_state = 0
        self.framesaveflag = 0
        self.Cal2_state = 0

    def init_ui(self):
        QApplication.setAttribute(QtCore.Qt.AA_EnableHighDpiScaling)
        QApplication.setStyle(QStyleFactory.create('Fusion'))
        # self.setFixedSize(900, 560)
        self.setBaseSize(900, 560)
        self.setWindowTitle("Netdemo")  # 设置窗口标题
        pg.setConfigOption('background', [255, 255, 255, 280])
        pg.setConfigOption('foreground', 'w')
        self.main_widget = QtWidgets.QWidget()
        self.main_layout = QtWidgets.QGridLayout()
        self.main_widget.setLayout(self.main_layout)
        self.left_widget = QtWidgets.QWidget()
        self.left_widget.setObjectName('left_widget')
        self.left_layout = QtWidgets.QGridLayout()
        self.left_widget.setLayout(self.left_layout)
        self.right_widget = QtWidgets.QWidget()
        self.right_widget.setObjectName('right_widget')
        self.right_layout = QtWidgets.QGridLayout()
        self.right_widget.setLayout(self.right_layout)
        self.main_layout.addWidget(self.left_widget, 0, 0, 10, 6)
        self.main_layout.addWidget(self.right_widget, 0, 7, 10, 2)
        self.setCentralWidget(self.main_widget)
        # 参数设置栏布局
        self.localaddress = QtWidgets.QLineEdit()
        self.filepath = QtWidgets.QLineEdit()
        self.localaddress.setText("192.168.1.190:6000")
        self.localaddress.setAlignment(Qt.AlignCenter)
        self.filepath.setText("FileGotten")
        self.filepath.setAlignment(Qt.AlignCenter)
        # self.filepath.setText("pic/")
        self.localaddress.setObjectName('inputtext')
        self.filepath.setObjectName('inputtext')
        self.label1 = QtWidgets.QLabel()
        self.label1.setText("Local IP Address：Port")
        self.label2 = QtWidgets.QLabel()
        self.label2.setText("File Save Path")
        self.label3 = QtWidgets.QLabel()
        self.label3.setText("Communication Protocol")
        self.label4 = QtWidgets.QLabel()
        self.label4.setText("RTSP")

        self.combo = QComboBox(self)
        self.combo.addItems(["UDP", "TCP"])
        # 选择当前窗口风格
        self.start = QtWidgets.QPushButton()
        self.start.setObjectName('start')
        self.start.setStyleSheet(
            '''QPushButton{background:#DF6D23;
              border-top-right-radius: 10px;
              border-bottom-right-radius: 10px;
              height:24px;}QPushButton:hover{background:red;}''')
        self.label4 = QtWidgets.QLabel()
        self.label4.setText("Command")
        self.textsent = QtWidgets.QTextEdit()
        self.label4.setObjectName("datasent")
        self.texttosent = QtWidgets.QLineEdit("Hello Netdemo!")
        self.send = QPushButton(qtawesome.icon('fa.upload', color='white'), "")
        self.send.setObjectName("send")
        self.send.setStyleSheet(
            '''QPushButton{background:#333333;
               font-weight:bold;
               color:white;
               border-bottom-right-radius: 10px;
               height:24px;}
               QPushButton:hover{background:black;}
               QPushButton:pressed{background:#aaaaaa;}
            ''')

        self.right_layout.addWidget(self.localaddress, 1, 0, 1, 5)
        self.right_layout.addWidget(self.filepath, 3, 0, 1, 5)
        self.right_layout.addWidget(self.combo, 5, 0, 1, 4)
        self.right_layout.addWidget(self.start, 5, 4, 1, 1)
        self.right_layout.addWidget(self.label1, 0, 0, 1, 5)
        self.right_layout.addWidget(self.label2, 2, 0, 1, 5)
        self.right_layout.addWidget(self.label3, 4, 0, 1, 5)
        self.right_layout.addWidget(self.label4, 6, 0, 1, 5)
        self.right_layout.addWidget(self.textsent, 7, 0, 8, 5)
        self.right_layout.addWidget(self.texttosent, 18, 0, 1, 4)
        self.right_layout.addWidget(self.send, 18, 4, 1, 1)
        self.right_widget.setStyleSheet('''
                    QLineEdit#inputtext{
                    width:160px;
                    border-radius:10px;
                    padding:2px 4px;
                    font-family:Arial;
                    height:20px;
                    }
                    QLineEdit{
                    color:white;
                    width:160px;
                    font-size:17px;
                    font-family:Arial;
                    border-bottom-left-radius: 10px;
                    min-height:25px;
                    background:#333333;
                    }
                    QComboBox{
                    width:160px;
                    font-family:Arial;
                    font-style: italic;
                    border-radius:10px;
                    padding:2px;
                    }
                    QLabel{
                    color:rgb(225,255,255,255);
                    font-size:17px;
                    font-weight:normal;
                    font-family:Arial;}

                     QLabel#datasent{
                     margin-top:50px;
                    color:rgb(225,255,255,255);
                    font-size:17px;
                    font-weight:normal;
                    font-family:Arial;}

                    QTextEdit{
                    background-color: #333333;
                    color:white;
                    font-size:17px;
                    font-family:Arial;
                     border-top-left-radius:10px;
                     border-top-right-radius:10px;
                     }
                     QScrollBar{background: black;width:15;}
                     QScrollBar::handle:vertical {background: white; min-height:20px; border-radius:5px;}
                     QScrollBar::sub-line:vertical {background:black;}


                ''')

        self.leftQtab = QTabWidget()
        self.pw = pg.PlotWidget()

        # Text part set
        self.text = QTextEdit()
        self.toaddress = QtWidgets.QLineEdit()
        self.toaddress.setText("192.168.1.199:5000")
        self.toaddress.setStyleSheet(
            '''
             color:white;
             width:160px;
             font-size:17px;
             font-family:Arial;
             border-radius: 10px;
             min-height:25px;
             background:#333333;
             text-align:center;
              ''')
        self.toaddress.setAlignment(Qt.AlignCenter)
        self.stop = QtWidgets.QPushButton(qtawesome.icon('fa.stop', color='white'), "")
        self.copy = QtWidgets.QPushButton(qtawesome.icon('fa.copy', color='white'), "")
        self.clear = QtWidgets.QPushButton(qtawesome.icon('fa.minus', color='white'), "")
        self.stop.setStyleSheet(
            '''QPushButton{background:#333333;
              border-radius: 15px;
              font-size:17px;
              font-weight:bold;
              min-width:80px;
              height:30px;}
              QPushButton:hover{background:#000000;}
              QPushButton:pressed{background:#aaaaaa;}
              ''')
        self.copy.setStyleSheet(
            '''QPushButton{background:#333333;
              border-radius: 15px;
              font-size:17px;
              font-weight:bold;
              min-width:80px;
              height:30px;}
              QPushButton:hover{background:#000000;}
              QPushButton:pressed{background:#aaaaaa;}
              ''')
        self.clear.setStyleSheet(
            '''QPushButton{background:#333333;
              border-radius: 15px;
              font-size:17px;
              font-weight:bold;
              min-width:80px;
              height:30px;}
              QPushButton:hover{background:#000000;}
              QPushButton:pressed{background:#aaaaaa;}
              ''')
        self.Text_B = QtWidgets.QWidget()
        self.Text_B_Layout = QtWidgets.QGridLayout()
        self.Text_B.setLayout(self.Text_B_Layout)
        self.Text_B_Layout.addWidget(self.text, 0, 0, 4, 8)
        self.Text_B_Layout.addWidget(self.stop, 9, 6, 1, 1)
        self.Text_B_Layout.addWidget(self.copy, 9, 7, 1, 1)
        self.Text_B_Layout.addWidget(self.clear, 9, 5, 1, 1)
        self.Text_B_Layout.addWidget(self.toaddress, 9, 0, 1, 5)
        # Video part set
        self.video = QtWidgets.QWidget()
        self.video_Layout = QtWidgets.QGridLayout()
        self.video.setLayout(self.video_Layout)
        self.videolabel = QtWidgets.QLabel()
        self.stop2 = QtWidgets.QPushButton(qtawesome.icon('fa.forward', color='white'), "")
        self.stop2.setStyleSheet(
            '''QPushButton{background:#333333;
              border-radius: 15px;
              font-size:17px;
              font-weight:bold;
              min-width:80px;
              height:30px;}
              QPushButton:hover{background:#000000;}
              QPushButton:pressed{background:#aaaaaa;}
              ''')
        self.copy2 = QtWidgets.QPushButton(qtawesome.icon('fa.copy', color='white'), "")
        self.copy2.setStyleSheet(
            '''QPushButton{background:#333333;
              border-radius: 15px;
              font-size:17px;
              font-weight:bold;
              min-width:80px;
              height:30px;}
              QPushButton:hover{background:#000000;}
              QPushButton:pressed{background:#aaaaaa;}
              ''')
        self.Cal2 = QtWidgets.QPushButton(qtawesome.icon('fa.star-o', color='white'), "")
        self.Cal2.setStyleSheet(
            '''QPushButton{background:#333333;
              border-radius: 15px;
              font-size:17px;
              font-weight:bold;
              min-width:80px;
              height:30px;}
              QPushButton:hover{background:#000000;}
              QPushButton:pressed{background:#aaaaaa;}
              ''')
        self.rtsp = QtWidgets.QLineEdit()
        self.rtsp.setText("admin:a1234567:192.168.1.10")
        self.rtsp.setAlignment(Qt.AlignCenter)
        self.rtsp.setStyleSheet(
            '''
             color:white;
             width:160px;
             font-size:17px;
             font-family:Arial;
             border-radius: 10px;
             min-height:25px;
             background:#333333;
              ''')
        self.video_Layout.addWidget(self.videolabel, 0, 0, 4, 8)
        self.video_Layout.addWidget(self.stop2, 9, 6, 1, 1)
        self.video_Layout.addWidget(self.copy2, 9, 7, 1, 1)
        self.video_Layout.addWidget(self.Cal2, 9, 5, 1, 1)
        self.video_Layout.addWidget(self.rtsp, 9, 0, 1, 5)

        # 将三个选项卡添加到顶层窗口中
        self.leftQtab.addTab(self.pw, "Chart")
        self.leftQtab.addTab(self.Text_B, "Text")
        self.leftQtab.addTab(self.video, "Video")
        self.left_layout.addWidget(self.leftQtab)
        self.left_widget.setStyleSheet('''
                           QTabWidget{
                           background-color: #000000;
                           font-family:Arial;
                            }
                           QTabBar::tab {
                            border: none;
                            min-width: 100px;
                            min-height:30px;
                            font-size: 16px;
                            color:white;
                            background-color:[255,255,255,0];
                            }
                            QTabBar::tab:selected, QTabBar::tab:hover {
                            background-color:gray;
                            }
                            QTabBar::tab:selected {
                            color:#000000;
                            font-weight:bold;
                            background-color:#FFFFFF;
                            }
                           QTextEdit{
                           background-color: #000000;
                           color:white;font-size:17px;
                           font-family:Arial;
                            }
                            QScrollBar{background: black;width:15;}
                            QScrollBar::handle:vertical {background: white; min-height:20px; border-radius:5px;}
                            QScrollBar::sub-line:vertical {background:black;}

                            QWidget#left_widget{
                            background:black;
                            border-top-left-radius:10px;
                            border-bottom-left-radius:10px;
                            }
                        ''')
        self.timer_start()
        self.setWindowOpacity(0.9)  # 设置窗口透明度
        self.setAttribute(QtCore.Qt.WA_TranslucentBackground)  # 设置窗口背景透明
        self.setWindowIcon(QIcon("virus2.ico"))
        self.start.clicked.connect(self.diagram_set)
        self.stop.clicked.connect(self.stop_deal)
        self.copy.clicked.connect(self.copy_deal)
        self.send.clicked.connect(self.send_deal)
        self.clear.clicked.connect(self.clear_deal)
        self.stop2.clicked.connect(self.stop2_deal)
        self.copy2.clicked.connect(self.copy2_deal)
        self.Cal2.clicked.connect(self.Cal2_deal)
        self.texttosent.returnPressed.connect(self.send_deal)

    def clear_deal(self):
        self.text.clear()

    def send_deal(self):
        # if (self.combo.currentText() == "UDP"):
        #     ss = self.toaddress.text().split(':', 2)
        #     ip_portd = (ss[0], int(ss[1]))
        #     self.server.sendto(self.texttosent.text().encode('utf-8'), ip_portd)
        # else:
        #     self.client_socket.send(self.texttosent.text().encode('utf-8'))
        self.dataoutQueue.put(self.texttosent.text())
        self.textsent.insertPlainText(str("%.3f" % time.perf_counter()) + ":" + self.texttosent.text() + "\r\n")
        self.textsent.moveCursor(QTextCursor.End)

    def diagram_set(self):
        with self.start_state.get_lock():
            self.start_state.value = (self.start_state.value + 1) % 2
        if (self.start_state.value == 1):
            if (self.combo.currentText() == "TCP"):
                # ip_port, mode, thread_flag, start_state, dataQueue
                ss = self.localaddress.text().split(':', 2)
                ip_port = (ss[0], int(ss[1]))
                ss = self.toaddress.text().split(':', 2)
                ip_portd = (ss[0], int(ss[1]))
                datareceiveprocess= Process(target=NetRec,
                                            args=(ip_port,ip_portd, 0,self.thread_flag,self.start_state,self.dataQueue,self.dataoutQueue))
                datareceiveprocess.daemon = True
                datareceiveprocess.start()
                self.start.setStyleSheet(
                    '''QPushButton{background:#6DDF6D;
                      border-top-right-radius: 10px;
                      border-bottom-right-radius: 10px;
                      height:24px;}QPushButton:hover{background:green;}''')
            elif (self.combo.currentText() == "UDP"):
                ss = self.localaddress.text().split(':', 2)
                ip_port = (ss[0], int(ss[1]))
                ss = self.toaddress.text().split(':', 2)
                ip_portd = (ss[0], int(ss[1]))
                datareceiveprocess = Process(target=NetRec,
                                             args=(ip_port, ip_portd,1, self.thread_flag, self.start_state, self.dataQueue,self.dataoutQueue))
                datareceiveprocess.daemon=True
                datareceiveprocess.start()
                self.start.setStyleSheet(
                    '''QPushButton{background:#6DDF6D;
                      border-top-right-radius: 10px;
                      border-bottom-right-radius: 10px;
                      height:24px;}QPushButton:hover{background:green;}''')
        else:
            self.start.setStyleSheet(
                '''QPushButton{background:#DF6D23;
                  border-top-right-radius: 10px;
                  border-bottom-right-radius: 10px;
                  height:24px;}QPushButton:hover{background:red;}''')
            with self.thread_flag.get_lock():
                self.thread_flag.value = 0

    def stop_deal(self):
        self.stop_state = (self.stop_state + 1) % 2
        if (self.stop_state == 1):
            # self.stop.setText("go on")
            self.stop.setIcon(qtawesome.icon('fa.forward', color='white'))
        else:
            self.stop.setIcon(qtawesome.icon('fa.stop', color='white'))

    def stop2_deal(self):
        self.stop2_state = (self.stop2_state + 1) % 2
        if (self.stop2_state == 1):
            # self.stop.setText("go on")
            thread2 = myThread2(self)
            thread2.setDaemon(True)
            thread2.start()
            # self.stop2.setIcon(qtawesome.icon('fa.forward', color='white'))
        # else:
        #     # self.stop2.setIcon(qtawesome.icon('fa.stop', color='white'))

    def copy_deal(self):
        pyperclip.copy(self.text.toPlainText())
        save_path = mkdir(self.filepath.text()) + "\\" + datetime.datetime.now().strftime(
            '%m%d-%H%M%S') + '.txt'  # 新创建的txt文件的存放路径
        file = open(save_path, 'w')
        file.write(self.text.toPlainText())  # msg也就是下面的Hello world

    def copy2_deal(self):
        if self.stop2_state == 1:
            self.framesaveflag = 1

    def Cal2_deal(self):
        self.Cal2_state = (self.Cal2_state + 1) % 2
        if (self.Cal2_state == 1):
            self.Cal2.setIcon(qtawesome.icon('fa.star', color='white'))
        else:
            self.Cal2.setIcon(qtawesome.icon('fa.star-o', color='white'))

    def timer_start(self):
        self.timer = QtCore.QTimer(self)
        self.timer.timeout.connect(self.redraw)
        self.timer.start(40)

    def redraw(self):
        while (self.dataQueue.qsize()):
            # print(self.dataQueue.get())
            sstr=self.dataQueue.get()
            # s = self.dataQueue.get().split()
            s=sstr.split()
            if (s[1] == 'D'):
                self.data_type = len(s) - 2
                if (self.data_type < 3):
                    self.data1_list.append(float(s[2]))
                    if (self.data_type == 2):
                        self.data2_list.append(float(s[3]))
                    self.time_list.append(float(s[0]))
            else:
                self.data_type = 0
                s = sstr.split(' ',1)
                self.orginrec_list.append(str(s[1]))
                self.time2_list.append(float(s[0]))

        try:
            if (self.thread_flag.value==1):
                if (len(self.time_list) | len(self.time2_list) > 0):
                    self.pw.clear()
                    if (len(self.time_list) <= 200):
                        self.pw.plot().setData(self.time_list, self.data1_list, pen='w')
                        if (self.data_type == 2):
                            self.pw.plot().setData(self.time_list, self.data2_list, pen='r')
                    else:
                        self.pw.plot().setData(self.time_list[-200:], self.data1_list[-200:], pen='w')
                        if (self.data_type == 2):
                            self.pw.plot().setData(self.time_list[-200:], self.data2_list[-200:], pen='r')
                    if (self.stop_state == 0):
                        if (self.printprogress < len(self.time_list)):
                            for i in range(self.printprogress, (len(self.time_list))):
                                if (self.data_type == 2):
                                    self.text.insertPlainText(
                                        str("%.3f" % self.time_list[i]).rjust(5, '0') + ": " + str(
                                            self.data1_list[i]).rjust(7, '0') + " " + str(self.data2_list[i]).rjust(7,
                                                                                                                    '0') + "\r\n")
                                elif (self.data_type == 1):
                                    self.text.insertPlainText(
                                        str("%.3f" % self.time_list[i]).rjust(5, '0') + ": " + str(
                                            self.data1_list[i]).rjust(7, '0') + "\r\n")
                            self.text.moveCursor(QTextCursor.End)
                            self.printprogress = len(self.time_list)
                if (self.stop_state == 0):
                    if (self.printprogress2 < len(self.time2_list)):
                        for i in range(self.printprogress2, (len(self.time2_list))):
                            self.text.insertPlainText(
                                str("%.3f" % self.time2_list[i]).rjust(5, '0') + ": " + self.orginrec_list[i] + "\r\n")
                        self.printprogress2 = len(self.time2_list)
                        self.text.moveCursor(QTextCursor.End)
        except Exception as e:
            print(e.print_exc())



def main():
    app = QtWidgets.QApplication(sys.argv)
    gui = MainUi()
    gui.show()
    sys.exit(app.exec_())


class myThread(threading.Thread):
    def __init__(self, socket, guii):
        threading.Thread.__init__(self)
        self.client_socket = socket
        self.gui = guii

    def run(self):
        while self.gui.thread_flag == 1:
            time.sleep(0.02)
            try:
                data = self.client_socket.recvfrom(BUFSIZE)
            except OSError:
                self.gui.thread_flag = 0
                self.gui.start_state = 0
                self.gui.start.setStyleSheet(
                    '''QPushButton{background:#DF6D23;
                      border-top-right-radius: 10px;
                      border-bottom-right-radius: 10px;
                      height:24px;}QPushButton:hover{background:red;}''')
                win32api.MessageBox(0, "Object Disconnected", "Error")
                self.gui.server.close()
            else:
                s = str(data[0].decode('utf-8')).split()
                if (s[0] == 'D'):
                    self.gui.data_type = len(s) - 1
                    if (self.gui.data_type < 3):
                        self.gui.data1_list.append(float(s[1]))
                        if (self.gui.data_type == 2):
                            self.gui.data2_list.append(float(s[2]))
                        self.gui.time_list.append(time.perf_counter())
                else:
                    self.gui.data_type = 0
                    self.gui.orginrec_list.append(str(data[0].decode('utf-8')))
                    self.gui.time2_list.append(time.perf_counter())
        self.client_socket.close()

def Netsend(socket, dataoutQueue,thread_flag,mode, ip_pord):
    # if (self.combo.currentText() == "UDP"):
    #     ss = self.toaddress.text().split(':', 2)
    #     ip_portd = (ss[0], int(ss[1]))
    #     self.server.sendto(self.texttosent.text().encode('utf-8'), ip_portd)
    # else:
    #     self.client_socket.send(self.texttosent.text().encode('utf-8'))
    # self.textsent.insertPlainText(str("%.3f" % time.perf_counter()) + ":" + self.texttosent.text() + "\r\n")
    # self.textsent.moveCursor(QTextCursor.End)
    while(thread_flag.value):
        time.sleep(0.001)
        try:
            if (mode==1):
                while(dataoutQueue.qsize()):
                    socket.sendto(dataoutQueue.get().encode('utf-8'),ip_pord)
            else:
                while (dataoutQueue.qsize()):
                    socket.send(dataoutQueue.get().encode('utf-8'))
        except AttributeError:
            win32api.MessageBox(0, "No Ethernet Object Connected", "Error")
        except OSError:
            win32api.MessageBox(0, "No Ethernet Object Connected", "Error")


def NetRec( ip_port,ip_portd, mode,thread_flag,start_state,dataQueue,dataoutQueue):
    if (mode==0):
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # tcp协议
        try:
           server.bind(ip_port)
        except OSError:
            server.close()
            win32api.MessageBox(0, "No Ethernet Object Connected", "Error")
            with start_state.get_lock():
                start_state.value = 0
            with thread_flag.get_lock():
                thread_flag.value = 0
        else:
            server.listen(128)
            client_socket, clientAddr = self.server.accept()
            with start_state.get_lock():
                start_state.value = 1
            with thread_flag.get_lock():
                thread_flag.value = 1
    elif(mode==1):
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # udp协议
        try:
            client_socket.bind(ip_port)
        except OSError:
            # print("Not connected")
            client_socket.close()
            win32api.MessageBox(0, "No Ethernet Object Connected", "Error")
            with start_state.get_lock():
                start_state.value = 0
            with thread_flag.get_lock():
                thread_flag.value = 0
        else:
            with start_state.get_lock():
                start_state.value = 1
            with thread_flag.get_lock():
                thread_flag.value = 1
    # socket, dataoutQueue, thread_flag, mode, ip_pord
    subthread = threading.Thread(target=Netsend, args=(client_socket,dataoutQueue,thread_flag,mode,ip_portd))
    subthread.setDaemon(True)
    subthread.start()

    while thread_flag.value == 1:
        # time.sleep(0.005)
        try:
            data = client_socket.recvfrom(BUFSIZE)
        except OSError:
            with start_state.get_lock():
                start_state.value = 0
            with thread_flag.get_lock():
                thread_flag.value = 0
            if(mode==0):
               server.close()
               client_socket.close()
            else:
               client_socket.close()
            win32api.MessageBox(0, "Object Disconnected", "Error")
        else:
            dataQueue.put(str(time.perf_counter())+" "+str(str(data[0].decode('utf-8'))))
    if (mode == 0):
        server.close()
        client_socket.close()
    else:
        client_socket.close()


class myThread2(threading.Thread):
    def __init__(self, guii):
        threading.Thread.__init__(self)
        self.gui = guii
        # self.cap = cv2.VideoCapture(self.gui.rtsp.text())
        self.time = 0
        loginfo = str(self.gui.rtsp.text()).split(':')
        ip = loginfo[2]  # 摄像头IP地址，要和本机IP在同一局域网
        name = loginfo[0]  # 管理员用户名
        pw = loginfo[1]  # 管理员密码
        if HKIPcamera.init(ip, name, pw) < 0:
            self.gui.stop2_state = 0
            self.gui.stop2.setIcon(qtawesome.icon('fa.forward', color='white'))
            win32api.MessageBox(0, "No Camera Found", "Error")
        else:
            self.gui.stop2.setIcon(qtawesome.icon('fa.stop', color='white'))

    def run(self):
        while self.gui.stop2_state == 1:
            self.image = np.array(HKIPcamera.getframe())
            show = cv2.resize(self.image, DIM2)
            show = cv2.cvtColor(show, cv2.COLOR_BGR2RGB)
            if self.gui.Cal2_state == 1:
                map1, map2 = cv2.fisheye.initUndistortRectifyMap(K2, D2, np.eye(3), K2, DIM2, cv2.CV_16SC2)
                show = cv2.remap(show, map1, map2, interpolation=cv2.INTER_LINEAR, borderMode=cv2.BORDER_CONSTANT)
                time.sleep(0.005)
            else:
                time.sleep(0.015)
            showImage = QtGui.QImage(show.data, show.shape[1], show.shape[0], QtGui.QImage.Format_RGB888)
            self.gui.videolabel.setPixmap(QtGui.QPixmap.fromImage(showImage))
            if self.gui.framesaveflag == 1:
                save_path = mkdir(self.gui.filepath.text()) + "\\" + datetime.datetime.now().strftime('%m%d-%H%M%S')
                cv2.imwrite(save_path + ".jpg", self.image)
                map1, map2 = cv2.fisheye.initUndistortRectifyMap(K, D, np.eye(3), K, DIM, cv2.CV_16SC2)
                undistorted_img = cv2.remap(self.image, map1, map2, interpolation=cv2.INTER_LINEAR,
                                            borderMode=cv2.BORDER_CONSTANT)
                cv2.imwrite(save_path + "d.jpg", undistorted_img)
                self.gui.framesaveflag = 0
        HKIPcamera.release()
        self.gui.stop2.setIcon(qtawesome.icon('fa.forward', color='white'))


def mkdir(pathfile):
    current_dir = os.path.abspath(os.path.dirname(__file__))
    path = current_dir + "\\" + pathfile
    path = path.strip()
    path = path.rstrip("\\")
    isExists = os.path.exists(path)
    if not isExists:
        os.makedirs(path)
    return path


if __name__ == '__main__':
    main()