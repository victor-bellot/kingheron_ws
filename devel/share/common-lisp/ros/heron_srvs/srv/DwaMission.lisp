; Auto-generated. Do not edit!


(cl:in-package heron_srvs-srv)


;//! \htmlinclude DwaMission-request.msg.html

(cl:defclass <DwaMission-request> (roslisp-msg-protocol:ros-message)
  ((goal_x
    :reader goal_x
    :initarg :goal_x
    :type cl:float
    :initform 0.0)
   (goal_y
    :reader goal_y
    :initarg :goal_y
    :type cl:float
    :initform 0.0))
)

(cl:defclass DwaMission-request (<DwaMission-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <DwaMission-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'DwaMission-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name heron_srvs-srv:<DwaMission-request> is deprecated: use heron_srvs-srv:DwaMission-request instead.")))

(cl:ensure-generic-function 'goal_x-val :lambda-list '(m))
(cl:defmethod goal_x-val ((m <DwaMission-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader heron_srvs-srv:goal_x-val is deprecated.  Use heron_srvs-srv:goal_x instead.")
  (goal_x m))

(cl:ensure-generic-function 'goal_y-val :lambda-list '(m))
(cl:defmethod goal_y-val ((m <DwaMission-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader heron_srvs-srv:goal_y-val is deprecated.  Use heron_srvs-srv:goal_y instead.")
  (goal_y m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <DwaMission-request>) ostream)
  "Serializes a message object of type '<DwaMission-request>"
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'goal_x))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'goal_y))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <DwaMission-request>) istream)
  "Deserializes a message object of type '<DwaMission-request>"
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'goal_x) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'goal_y) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<DwaMission-request>)))
  "Returns string type for a service object of type '<DwaMission-request>"
  "heron_srvs/DwaMissionRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'DwaMission-request)))
  "Returns string type for a service object of type 'DwaMission-request"
  "heron_srvs/DwaMissionRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<DwaMission-request>)))
  "Returns md5sum for a message object of type '<DwaMission-request>"
  "d4f8fbf2f34a6f33bd852a1c941167c4")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'DwaMission-request)))
  "Returns md5sum for a message object of type 'DwaMission-request"
  "d4f8fbf2f34a6f33bd852a1c941167c4")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<DwaMission-request>)))
  "Returns full string definition for message of type '<DwaMission-request>"
  (cl:format cl:nil "float32 goal_x~%float32 goal_y~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'DwaMission-request)))
  "Returns full string definition for message of type 'DwaMission-request"
  (cl:format cl:nil "float32 goal_x~%float32 goal_y~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <DwaMission-request>))
  (cl:+ 0
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <DwaMission-request>))
  "Converts a ROS message object to a list"
  (cl:list 'DwaMission-request
    (cl:cons ':goal_x (goal_x msg))
    (cl:cons ':goal_y (goal_y msg))
))
;//! \htmlinclude DwaMission-response.msg.html

(cl:defclass <DwaMission-response> (roslisp-msg-protocol:ros-message)
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

(cl:defclass DwaMission-response (<DwaMission-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <DwaMission-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'DwaMission-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name heron_srvs-srv:<DwaMission-response> is deprecated: use heron_srvs-srv:DwaMission-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <DwaMission-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader heron_srvs-srv:success-val is deprecated.  Use heron_srvs-srv:success instead.")
  (success m))

(cl:ensure-generic-function 'message-val :lambda-list '(m))
(cl:defmethod message-val ((m <DwaMission-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader heron_srvs-srv:message-val is deprecated.  Use heron_srvs-srv:message instead.")
  (message m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <DwaMission-response>) ostream)
  "Serializes a message object of type '<DwaMission-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'message))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'message))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <DwaMission-response>) istream)
  "Deserializes a message object of type '<DwaMission-response>"
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
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<DwaMission-response>)))
  "Returns string type for a service object of type '<DwaMission-response>"
  "heron_srvs/DwaMissionResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'DwaMission-response)))
  "Returns string type for a service object of type 'DwaMission-response"
  "heron_srvs/DwaMissionResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<DwaMission-response>)))
  "Returns md5sum for a message object of type '<DwaMission-response>"
  "d4f8fbf2f34a6f33bd852a1c941167c4")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'DwaMission-response)))
  "Returns md5sum for a message object of type 'DwaMission-response"
  "d4f8fbf2f34a6f33bd852a1c941167c4")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<DwaMission-response>)))
  "Returns full string definition for message of type '<DwaMission-response>"
  (cl:format cl:nil "bool success~%string message~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'DwaMission-response)))
  "Returns full string definition for message of type 'DwaMission-response"
  (cl:format cl:nil "bool success~%string message~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <DwaMission-response>))
  (cl:+ 0
     1
     4 (cl:length (cl:slot-value msg 'message))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <DwaMission-response>))
  "Converts a ROS message object to a list"
  (cl:list 'DwaMission-response
    (cl:cons ':success (success msg))
    (cl:cons ':message (message msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'DwaMission)))
  'DwaMission-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'DwaMission)))
  'DwaMission-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'DwaMission)))
  "Returns string type for a service object of type '<DwaMission>"
  "heron_srvs/DwaMission")