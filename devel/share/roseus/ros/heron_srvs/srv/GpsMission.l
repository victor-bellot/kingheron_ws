;; Auto-generated. Do not edit!


(when (boundp 'heron_srvs::GpsMission)
  (if (not (find-package "HERON_SRVS"))
    (make-package "HERON_SRVS"))
  (shadow 'GpsMission (find-package "HERON_SRVS")))
(unless (find-package "HERON_SRVS::GPSMISSION")
  (make-package "HERON_SRVS::GPSMISSION"))
(unless (find-package "HERON_SRVS::GPSMISSIONREQUEST")
  (make-package "HERON_SRVS::GPSMISSIONREQUEST"))
(unless (find-package "HERON_SRVS::GPSMISSIONRESPONSE")
  (make-package "HERON_SRVS::GPSMISSIONRESPONSE"))

(in-package "ROS")





(defclass heron_srvs::GpsMissionRequest
  :super ros::object
  :slots (_gps_x _gps_y _vel ))

(defmethod heron_srvs::GpsMissionRequest
  (:init
   (&key
    ((:gps_x __gps_x) 0.0)
    ((:gps_y __gps_y) 0.0)
    ((:vel __vel) 0.0)
    )
   (send-super :init)
   (setq _gps_x (float __gps_x))
   (setq _gps_y (float __gps_y))
   (setq _vel (float __vel))
   self)
  (:gps_x
   (&optional __gps_x)
   (if __gps_x (setq _gps_x __gps_x)) _gps_x)
  (:gps_y
   (&optional __gps_y)
   (if __gps_y (setq _gps_y __gps_y)) _gps_y)
  (:vel
   (&optional __vel)
   (if __vel (setq _vel __vel)) _vel)
  (:serialization-length
   ()
   (+
    ;; float32 _gps_x
    4
    ;; float32 _gps_y
    4
    ;; float32 _vel
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _gps_x
       (sys::poke _gps_x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _gps_y
       (sys::poke _gps_y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _vel
       (sys::poke _vel (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _gps_x
     (setq _gps_x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _gps_y
     (setq _gps_y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _vel
     (setq _vel (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(defclass heron_srvs::GpsMissionResponse
  :super ros::object
  :slots (_success _message ))

(defmethod heron_srvs::GpsMissionResponse
  (:init
   (&key
    ((:success __success) nil)
    ((:message __message) "")
    )
   (send-super :init)
   (setq _success __success)
   (setq _message (string __message))
   self)
  (:success
   (&optional __success)
   (if __success (setq _success __success)) _success)
  (:message
   (&optional __message)
   (if __message (setq _message __message)) _message)
  (:serialization-length
   ()
   (+
    ;; bool _success
    1
    ;; string _message
    4 (length _message)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _success
       (if _success (write-byte -1 s) (write-byte 0 s))
     ;; string _message
       (write-long (length _message) s) (princ _message s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _success
     (setq _success (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; string _message
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _message (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass heron_srvs::GpsMission
  :super ros::object
  :slots ())

(setf (get heron_srvs::GpsMission :md5sum-) "48a816069f77785b03ce1850b9079d52")
(setf (get heron_srvs::GpsMission :datatype-) "heron_srvs/GpsMission")
(setf (get heron_srvs::GpsMission :request) heron_srvs::GpsMissionRequest)
(setf (get heron_srvs::GpsMission :response) heron_srvs::GpsMissionResponse)

(defmethod heron_srvs::GpsMissionRequest
  (:response () (instance heron_srvs::GpsMissionResponse :init)))

(setf (get heron_srvs::GpsMissionRequest :md5sum-) "48a816069f77785b03ce1850b9079d52")
(setf (get heron_srvs::GpsMissionRequest :datatype-) "heron_srvs/GpsMissionRequest")
(setf (get heron_srvs::GpsMissionRequest :definition-)
      "float32 gps_x
float32 gps_y
float32 vel
---
bool success
string message

")

(setf (get heron_srvs::GpsMissionResponse :md5sum-) "48a816069f77785b03ce1850b9079d52")
(setf (get heron_srvs::GpsMissionResponse :datatype-) "heron_srvs/GpsMissionResponse")
(setf (get heron_srvs::GpsMissionResponse :definition-)
      "float32 gps_x
float32 gps_y
float32 vel
---
bool success
string message

")



(provide :heron_srvs/GpsMission "48a816069f77785b03ce1850b9079d52")


