;; Auto-generated. Do not edit!


(when (boundp 'heron_srvs::DwaMission)
  (if (not (find-package "HERON_SRVS"))
    (make-package "HERON_SRVS"))
  (shadow 'DwaMission (find-package "HERON_SRVS")))
(unless (find-package "HERON_SRVS::DWAMISSION")
  (make-package "HERON_SRVS::DWAMISSION"))
(unless (find-package "HERON_SRVS::DWAMISSIONREQUEST")
  (make-package "HERON_SRVS::DWAMISSIONREQUEST"))
(unless (find-package "HERON_SRVS::DWAMISSIONRESPONSE")
  (make-package "HERON_SRVS::DWAMISSIONRESPONSE"))

(in-package "ROS")





(defclass heron_srvs::DwaMissionRequest
  :super ros::object
  :slots (_goal_x _goal_y ))

(defmethod heron_srvs::DwaMissionRequest
  (:init
   (&key
    ((:goal_x __goal_x) 0.0)
    ((:goal_y __goal_y) 0.0)
    )
   (send-super :init)
   (setq _goal_x (float __goal_x))
   (setq _goal_y (float __goal_y))
   self)
  (:goal_x
   (&optional __goal_x)
   (if __goal_x (setq _goal_x __goal_x)) _goal_x)
  (:goal_y
   (&optional __goal_y)
   (if __goal_y (setq _goal_y __goal_y)) _goal_y)
  (:serialization-length
   ()
   (+
    ;; float32 _goal_x
    4
    ;; float32 _goal_y
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _goal_x
       (sys::poke _goal_x (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _goal_y
       (sys::poke _goal_y (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _goal_x
     (setq _goal_x (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _goal_y
     (setq _goal_y (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(defclass heron_srvs::DwaMissionResponse
  :super ros::object
  :slots (_success _message ))

(defmethod heron_srvs::DwaMissionResponse
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

(defclass heron_srvs::DwaMission
  :super ros::object
  :slots ())

(setf (get heron_srvs::DwaMission :md5sum-) "d4f8fbf2f34a6f33bd852a1c941167c4")
(setf (get heron_srvs::DwaMission :datatype-) "heron_srvs/DwaMission")
(setf (get heron_srvs::DwaMission :request) heron_srvs::DwaMissionRequest)
(setf (get heron_srvs::DwaMission :response) heron_srvs::DwaMissionResponse)

(defmethod heron_srvs::DwaMissionRequest
  (:response () (instance heron_srvs::DwaMissionResponse :init)))

(setf (get heron_srvs::DwaMissionRequest :md5sum-) "d4f8fbf2f34a6f33bd852a1c941167c4")
(setf (get heron_srvs::DwaMissionRequest :datatype-) "heron_srvs/DwaMissionRequest")
(setf (get heron_srvs::DwaMissionRequest :definition-)
      "float32 goal_x
float32 goal_y
---
bool success
string message

")

(setf (get heron_srvs::DwaMissionResponse :md5sum-) "d4f8fbf2f34a6f33bd852a1c941167c4")
(setf (get heron_srvs::DwaMissionResponse :datatype-) "heron_srvs/DwaMissionResponse")
(setf (get heron_srvs::DwaMissionResponse :definition-)
      "float32 goal_x
float32 goal_y
---
bool success
string message

")



(provide :heron_srvs/DwaMission "d4f8fbf2f34a6f33bd852a1c941167c4")


