; Auto-generated. Do not edit!


(cl:in-package heron_srvs-srv)


;//! \htmlinclude AskModel-request.msg.html

(cl:defclass <AskModel-request> (roslisp-msg-protocol:ros-message)
  ((model_name
    :reader model_name
    :initarg :model_name
    :type cl:string
    :initform "")
   (position
    :reader position
    :initarg :position
    :type geometry_msgs-msg:Point
    :initform (cl:make-instance 'geometry_msgs-msg:Point)))
)

(cl:defclass AskModel-request (<AskModel-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <AskModel-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'AskModel-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name heron_srvs-srv:<AskModel-request> is deprecated: use heron_srvs-srv:AskModel-request instead.")))

(cl:ensure-generic-function 'model_name-val :lambda-list '(m))
(cl:defmethod model_name-val ((m <AskModel-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader heron_srvs-srv:model_name-val is deprecated.  Use heron_srvs-srv:model_name instead.")
  (model_name m))

(cl:ensure-generic-function 'position-val :lambda-list '(m))
(cl:defmethod position-val ((m <AskModel-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader heron_srvs-srv:position-val is deprecated.  Use heron_srvs-srv:position instead.")
  (position m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <AskModel-request>) ostream)
  "Serializes a message object of type '<AskModel-request>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'model_name))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'model_name))
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'position) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <AskModel-request>) istream)
  "Deserializes a message object of type '<AskModel-request>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'model_name) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'model_name) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'position) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<AskModel-request>)))
  "Returns string type for a service object of type '<AskModel-request>"
  "heron_srvs/AskModelRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'AskModel-request)))
  "Returns string type for a service object of type 'AskModel-request"
  "heron_srvs/AskModelRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<AskModel-request>)))
  "Returns md5sum for a message object of type '<AskModel-request>"
  "83bea2f2ad45b48dc9b07a242180ea66")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'AskModel-request)))
  "Returns md5sum for a message object of type 'AskModel-request"
  "83bea2f2ad45b48dc9b07a242180ea66")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<AskModel-request>)))
  "Returns full string definition for message of type '<AskModel-request>"
  (cl:format cl:nil "string model_name~%geometry_msgs/Point position~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'AskModel-request)))
  "Returns full string definition for message of type 'AskModel-request"
  (cl:format cl:nil "string model_name~%geometry_msgs/Point position~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <AskModel-request>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'model_name))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'position))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <AskModel-request>))
  "Converts a ROS message object to a list"
  (cl:list 'AskModel-request
    (cl:cons ':model_name (model_name msg))
    (cl:cons ':position (position msg))
))
;//! \htmlinclude AskModel-response.msg.html

(cl:defclass <AskModel-response> (roslisp-msg-protocol:ros-message)
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

(cl:defclass AskModel-response (<AskModel-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <AskModel-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'AskModel-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name heron_srvs-srv:<AskModel-response> is deprecated: use heron_srvs-srv:AskModel-response instead.")))

(cl:ensure-generic-function 'success-val :lambda-list '(m))
(cl:defmethod success-val ((m <AskModel-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader heron_srvs-srv:success-val is deprecated.  Use heron_srvs-srv:success instead.")
  (success m))

(cl:ensure-generic-function 'message-val :lambda-list '(m))
(cl:defmethod message-val ((m <AskModel-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader heron_srvs-srv:message-val is deprecated.  Use heron_srvs-srv:message instead.")
  (message m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <AskModel-response>) ostream)
  "Serializes a message object of type '<AskModel-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'success) 1 0)) ostream)
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'message))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'message))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <AskModel-response>) istream)
  "Deserializes a message object of type '<AskModel-response>"
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
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<AskModel-response>)))
  "Returns string type for a service object of type '<AskModel-response>"
  "heron_srvs/AskModelResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'AskModel-response)))
  "Returns string type for a service object of type 'AskModel-response"
  "heron_srvs/AskModelResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<AskModel-response>)))
  "Returns md5sum for a message object of type '<AskModel-response>"
  "83bea2f2ad45b48dc9b07a242180ea66")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'AskModel-response)))
  "Returns md5sum for a message object of type 'AskModel-response"
  "83bea2f2ad45b48dc9b07a242180ea66")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<AskModel-response>)))
  "Returns full string definition for message of type '<AskModel-response>"
  (cl:format cl:nil "bool success~%string message~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'AskModel-response)))
  "Returns full string definition for message of type 'AskModel-response"
  (cl:format cl:nil "bool success~%string message~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <AskModel-response>))
  (cl:+ 0
     1
     4 (cl:length (cl:slot-value msg 'message))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <AskModel-response>))
  "Converts a ROS message object to a list"
  (cl:list 'AskModel-response
    (cl:cons ':success (success msg))
    (cl:cons ':message (message msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'AskModel)))
  'AskModel-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'AskModel)))
  'AskModel-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'AskModel)))
  "Returns string type for a service object of type '<AskModel>"
  "heron_srvs/AskModel")