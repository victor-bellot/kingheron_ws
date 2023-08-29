// Auto-generated. Do not edit!

// (in-package heron_srvs.srv)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------


//-----------------------------------------------------------

class GpsMissionRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.gps_x = null;
      this.gps_y = null;
      this.vel = null;
    }
    else {
      if (initObj.hasOwnProperty('gps_x')) {
        this.gps_x = initObj.gps_x
      }
      else {
        this.gps_x = 0.0;
      }
      if (initObj.hasOwnProperty('gps_y')) {
        this.gps_y = initObj.gps_y
      }
      else {
        this.gps_y = 0.0;
      }
      if (initObj.hasOwnProperty('vel')) {
        this.vel = initObj.vel
      }
      else {
        this.vel = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type GpsMissionRequest
    // Serialize message field [gps_x]
    bufferOffset = _serializer.float32(obj.gps_x, buffer, bufferOffset);
    // Serialize message field [gps_y]
    bufferOffset = _serializer.float32(obj.gps_y, buffer, bufferOffset);
    // Serialize message field [vel]
    bufferOffset = _serializer.float32(obj.vel, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type GpsMissionRequest
    let len;
    let data = new GpsMissionRequest(null);
    // Deserialize message field [gps_x]
    data.gps_x = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [gps_y]
    data.gps_y = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [vel]
    data.vel = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 12;
  }

  static datatype() {
    // Returns string type for a service object
    return 'heron_srvs/GpsMissionRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '4d853c08e41f71ed018043f1bc42f7ea';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float32 gps_x
    float32 gps_y
    float32 vel
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new GpsMissionRequest(null);
    if (msg.gps_x !== undefined) {
      resolved.gps_x = msg.gps_x;
    }
    else {
      resolved.gps_x = 0.0
    }

    if (msg.gps_y !== undefined) {
      resolved.gps_y = msg.gps_y;
    }
    else {
      resolved.gps_y = 0.0
    }

    if (msg.vel !== undefined) {
      resolved.vel = msg.vel;
    }
    else {
      resolved.vel = 0.0
    }

    return resolved;
    }
};

class GpsMissionResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.success = null;
      this.message = null;
    }
    else {
      if (initObj.hasOwnProperty('success')) {
        this.success = initObj.success
      }
      else {
        this.success = false;
      }
      if (initObj.hasOwnProperty('message')) {
        this.message = initObj.message
      }
      else {
        this.message = '';
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type GpsMissionResponse
    // Serialize message field [success]
    bufferOffset = _serializer.bool(obj.success, buffer, bufferOffset);
    // Serialize message field [message]
    bufferOffset = _serializer.string(obj.message, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type GpsMissionResponse
    let len;
    let data = new GpsMissionResponse(null);
    // Deserialize message field [success]
    data.success = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [message]
    data.message = _deserializer.string(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.message.length;
    return length + 5;
  }

  static datatype() {
    // Returns string type for a service object
    return 'heron_srvs/GpsMissionResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '937c9679a518e3a18d831e57125ea522';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    bool success
    string message
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new GpsMissionResponse(null);
    if (msg.success !== undefined) {
      resolved.success = msg.success;
    }
    else {
      resolved.success = false
    }

    if (msg.message !== undefined) {
      resolved.message = msg.message;
    }
    else {
      resolved.message = ''
    }

    return resolved;
    }
};

module.exports = {
  Request: GpsMissionRequest,
  Response: GpsMissionResponse,
  md5sum() { return '48a816069f77785b03ce1850b9079d52'; },
  datatype() { return 'heron_srvs/GpsMission'; }
};
