from multiprocessing import Process, Queue,Value
import multiprocessing as mp
import HKIPcamera
import time
import cv2
import datetime
import os

DIM = (1280, 720)
K = np.array([[911.1546799130407, 0.0, 636.1179417117924], [0.0, 909.46867680695, 362.22498374331553], [0.0, 0.0, 1.0]])
D = np.array([[-0.20518436786315236], [0.13086850373804892], [-0.38920896106289166], [0.3717694153970601]])
DIM2 = (640, 450)
K2 = np.array(
    [[454.6079430869957, 0.0, 318.3465274330906], [0.0, 566.2354983838005, 226.9368868881296], [0.0, 0.0, 1.0]])
D2 = np.array([[-0.20234958495037134], [0.1432242196055454], [-0.4347283951611514], [0.4076209075432136]])

def PicRec(loginfo,filepath,stop2_state,Cal2_state,framesaveflag,PicQueue):
        loginfoready=str(loginfo).split(':')
        ip = loginfoready[2]  # 摄像头IP地址，要和本机IP在同一局域网
        name = loginfoready[0]  # 管理员用户名
        pw = loginfor[1] # 管理员密码
        if HKIPcamera.init(ip, name, pw)<0:
           with stop2_state.get_lock():
                stop2_state.value=0
           # self.gui.stop2.setIcon(qtawesome.icon('fa.forward', color='white'))
           win32api.MessageBox(0, "No Camera Found", "Error")
        else:
            with stop2_state.get_lock():
                stop2_state.value = 1
        #    self.gui.stop2.setIcon(qtawesome.icon('fa.stop', color='white'))
        while stop2_state.value==1:
            image=np.array(HKIPcamera.getframe())
            show = cv2.resize(image, DIM2)
            show = cv2.cvtColor(show, cv2.COLOR_BGR2RGB)
            if Cal2_state.value==1:
                map1, map2 = cv2.fisheye.initUndistortRectifyMap(K2, D2, np.eye(3), K2, DIM2, cv2.CV_16SC2)
                show = cv2.remap(show, map1, map2, interpolation=cv2.INTER_LINEAR, borderMode=cv2.BORDER_CONSTANT)
                time.sleep(0.005)
            else:
                time.sleep(0.015)
            showImage = QtGui.QImage(show, show.shape[1], show.shape[0], QtGui.QImage.Format_RGB888)
            if(PicQueue.qsize()!=0):
                PicQueue.put(QtGui.QPixmap.fromImage(showImage))
            if framesaveflag.value==1:
                save_path = mkdir(filepath) + "\\" + datetime.datetime.now().strftime('%m%d-%H%M%S')
                cv2.imwrite(save_path+".jpg", image)
                map1, map2 = cv2.fisheye.initUndistortRectifyMap(K, D, np.eye(3), K, DIM, cv2.CV_16SC2)
                undistorted_img = cv2.remap(image, map1, map2, interpolation=cv2.INTER_LINEAR, borderMode=cv2.BORDER_CONSTANT)
                cv2.imwrite(save_path + "d.jpg", undistorted_img)
                with framesaveflag.get_clock():
                    framesaveflag.value=0
        HKIPcamera.release()
        # self.gui.stop2.setIcon(qtawesome.icon('fa.forward', color='white'))

def mkdir(pathfile):
    current_dir = os.path.abspath(os.path.dirname(__file__))
    path = current_dir + "\\" + pathfile
    path = path.strip()
    path = path.rstrip("\\")
    isExists = os.path.exists(path)
    if not isExists:
        os.makedirs(path)
    return path
