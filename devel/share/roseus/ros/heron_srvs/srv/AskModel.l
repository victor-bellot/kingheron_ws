;; Auto-generated. Do not edit!


(when (boundp 'heron_srvs::AskModel)
  (if (not (find-package "HERON_SRVS"))
    (make-package "HERON_SRVS"))
  (shadow 'AskModel (find-package "HERON_SRVS")))
(unless (find-package "HERON_SRVS::ASKMODEL")
  (make-package "HERON_SRVS::ASKMODEL"))
(unless (find-package "HERON_SRVS::ASKMODELREQUEST")
  (make-package "HERON_SRVS::ASKMODELREQUEST"))
(unless (find-package "HERON_SRVS::ASKMODELRESPONSE")
  (make-package "HERON_SRVS::ASKMODELRESPONSE"))

(in-package "ROS")

(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))




(defclass heron_srvs::AskModelRequest
  :super ros::object
  :slots (_model_name _position ))

(defmethod heron_srvs::AskModelRequest
  (:init
   (&key
    ((:model_name __model_name) "")
    ((:position __position) (instance geometry_msgs::Point :init))
    )
   (send-super :init)
   (setq _model_name (string __model_name))
   (setq _position __position)
   self)
  (:model_name
   (&optional __model_name)
   (if __model_name (setq _model_name __model_name)) _model_name)
  (:position
   (&rest __position)
   (if (keywordp (car __position))
       (send* _position __position)
     (progn
       (if __position (setq _position (car __position)))
       _position)))
  (:serialization-length
   ()
   (+
    ;; string _model_name
    4 (length _model_name)
    ;; geometry_msgs/Point _position
    (send _position :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _model_name
       (write-long (length _model_name) s) (princ _model_name s)
     ;; geometry_msgs/Point _position
       (send _position :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _model_name
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _model_name (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; geometry_msgs/Point _position
     (send _position :deserialize buf ptr-) (incf ptr- (send _position :serialization-length))
   ;;
   self)
  )

(defclass heron_srvs::AskModelResponse
  :super ros::object
  :slots (_success _message ))

(defmethod heron_srvs::AskModelResponse
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

(defclass heron_srvs::AskModel
  :super ros::object
  :slots ())

(setf (get heron_srvs::AskModel :md5sum-) "83bea2f2ad45b48dc9b07a242180ea66")
(setf (get heron_srvs::AskModel :datatype-) "heron_srvs/AskModel")
(setf (get heron_srvs::AskModel :request) heron_srvs::AskModelRequest)
(setf (get heron_srvs::AskModel :response) heron_srvs::AskModelResponse)

(defmethod heron_srvs::AskModelRequest
  (:response () (instance heron_srvs::AskModelResponse :init)))

(setf (get heron_srvs::AskModelRequest :md5sum-) "83bea2f2ad45b48dc9b07a242180ea66")
(setf (get heron_srvs::AskModelRequest :datatype-) "heron_srvs/AskModelRequest")
(setf (get heron_srvs::AskModelRequest :definition-)
      "string model_name
geometry_msgs/Point position

================================================================================
MSG: geometry_msgs/Point
# This contains the position of a point in free space
float64 x
float64 y
float64 z
---
bool success
string message

")

(setf (get heron_srvs::AskModelResponse :md5sum-) "83bea2f2ad45b48dc9b07a242180ea66")
(setf (get heron_srvs::AskModelResponse :datatype-) "heron_srvs/AskModelResponse")
(setf (get heron_srvs::AskModelResponse :definition-)
      "string model_name
geometry_msgs/Point position

================================================================================
MSG: geometry_msgs/Point
# This contains the position of a point in free space
float64 x
float64 y
float64 z
---
bool success
string message

")



(provide :heron_srvs/AskModel "83bea2f2ad45b48dc9b07a242180ea66")


