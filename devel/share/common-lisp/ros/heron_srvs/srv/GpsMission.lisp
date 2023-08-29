; Auto-generated. Do not edit!


(cl:in-package heron_srvs-srv)


;//! \htmlinclude GpsMission-request.msg.html

(cl:defclass <GpsMission-request> (roslisp-msg-protocol:ros-message)
  ((gps_x
    :reader gps_x
    :initarg :gps_x
    :type cl:float
    :initform 0.0)
   (gps_y
    :reader gps_y
    :initarg :gps_y
    :type cl:float
    :initform 0.0)
   (vel
    :reader vel
    :initarg :vel
    :type cl:float
    :initform 0.0))
)

(cl:defclass GpsMission-request (<GpsMission-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <GpsMission-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'GpsMission-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name heron_srvs-srv:<GpsMission-request> is deprecated: use heron_srvs-srv:GpsMission-request instead.")))

(cl:ensure-generic-function 'gps_x-val :lambda-list '(m))
(cl:defmethod gps_x-val ((m <GpsMission-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader heron_srvs-srv:gps_x-val is deprecated.  Use heron_srvs-srv:gps_x instead.")
  (gps_x m))

(cl:ensure-generic-function 'gps_y-val :lambda-list '(m))
(cl:defmethod gps_y-val ((m <GpsMission-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader heron_srvs-srv:gps_y-val is deprecated.  Use heron_srvs-srv:gps_y instead.")
  (gps_y m))

(cl:ensure-generic-function 'vel-val :lambda-list '(m))
(cl:defmethod vel-val ((m <GpsMission-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader heron_srvs-srv:vel-val is deprecated.  Use heron_srvs-srv:vel instead.")
  (vel m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <GpsMission-request>) ostream)
  "Serializes a message object of type '<GpsMission-request>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'gps_x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'gps_y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'vel))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <GpsMission-request>) istream)
  "Deserializes a message object of type '<GpsMission-request>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'gps_x) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'gps_y) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'vel) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<GpsMission-request>)))
  "Returns string type for a service object of type '<GpsMission-request>"
  "heron_srvs/GpsMissionRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'GpsMission-request)))
  "Returns string type for a service object of type 'GpsMission-request"
  "heron_srvs/GpsMissionRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<GpsMission-request>)))
  "Returns md5sum for a message object of type '<GpsMission-request>"
  "48a816069f77785b03ce1850b9079d52")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'GpsMission-request)))
  "Returns md5sum for a message object of type 'GpsMission-request"
  "48a816069f77785b03ce1850b9079d52")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<GpsMission-request>)))
  "Returns full string definition for message of type '<GpsMission-request>"
  (cl:format cl:nil "float32 gps_x~%float32 gps_y~%float32 vel~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'GpsMission-request)))
  "Returns full string definition for message of type 'GpsMission-request"
  (cl:format cl:nil "float32 gps_x~%float32 gps_y~%float32 vel~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <GpsMission-request>))
  (cl:+ 0
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <GpsMission-request>))
  "Converts a ROS message object to a list"
  (cl:list 'GpsMission-request
    (cl:cons ':gps_x (gps_x msg))
    (cl:cons ':gps_y (gps_y msg))
    (cl:cons ':vel (vel msg))
))
;//! \htmlinclude GpsMission-response.msg.html

(cl:defclass <GpsMission-response> (roslisp-msg-protocol:ros-message)
  ((success
    :reader success
    :initarg :success
    :type cl:boolean
    :initform cl:nil)
   (message
    :reader message
    :initarg :message
    :type cl:string
    :initform ""))
)

(cl:defclass GpsMission-response (<GpsMission-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <GpsMission-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'GpsMission-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name heron_srvs-srv:<GpsMission-response> is deprecated: use heron_srvs-srv:GpsMission-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <GpsMission-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader heron_srvs-srv:success-val is deprecated.  Use heron_srvs-srv:success instead.")
  (success m))

(cl:ensure-generic-function 'message-val :lambda-list '(m))
(cl:defmethod message-val ((m <GpsMission-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader heron_srvs-srv:message-val is deprecated.  Use heron_srvs-srv:message instead.")
  (message m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <GpsMission-response>) ostream)
  "Serializes a message object of type '<GpsMission-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'message))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'message))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <GpsMission-response>) istream)
  "Deserializes a message object of type '<GpsMission-response>"
    (cl:setf (cl:slot-value msg 'success) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'message) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'message) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<GpsMission-response>)))
  "Returns string type for a service object of type '<GpsMission-response>"
  "heron_srvs/GpsMissionResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'GpsMission-response)))
  "Returns string type for a service object of type 'GpsMission-response"
  "heron_srvs/GpsMissionResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<GpsMission-response>)))
  "Returns md5sum for a message object of type '<GpsMission-response>"
  "48a816069f77785b03ce1850b9079d52")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'GpsMission-response)))
  "Returns md5sum for a message object of type 'GpsMission-response"
  "48a816069f77785b03ce1850b9079d52")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<GpsMission-response>)))
  "Returns full string definition for message of type '<GpsMission-response>"
  (cl:format cl:nil "bool success~%string message~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'GpsMission-response)))
  "Returns full string definition for message of type 'GpsMission-response"
  (cl:format cl:nil "bool success~%string message~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <GpsMission-response>))
  (cl:+ 0
     1
     4 (cl:length (cl:slot-value msg 'message))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <GpsMission-response>))
  "Converts a ROS message object to a list"
  (cl:list 'GpsMission-response
    (cl:cons ':success (success msg))
    (cl:cons ':message (message msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'GpsMission)))
  'GpsMission-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'GpsMission)))
  'GpsMission-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'GpsMission)))
  "Returns string type for a service object of type '<GpsMission>"
  "heron_srvs/GpsMission")