const mongoose = require("mongoose")
const express = require("express")
const router = express.Router()
const objects = require("../../models/objects")
const multer = require("multer")

const uploadPath = "/Users/gregcoppola/uploads"
const storage = multer.diskStorage({
  destination(request, file, cb) {
    cb(null, uploadPath)
  },
  filename(request, file, cb) {
    // 1. Start with a uuid.
    var result = uuid.v1()
    // 2. If there is an extension, add it.
    const parts = file.originalname.split(".")
    if (parts.size > 1) {
      result += "."
      result += parts[parts.length - 1]
    }
    cb(null, result)
  }
})

const fileFilter = (request, file, cb) => {
  if (true) {
    // if (file.mimetype == "image/jpeg" || file.mimetype == "image/png") {
    cb(null, true)
  } else {
    cb(null, false)
  }
}

const upload = multer({
  storage: storage,
  limits: {
    fileSize: 1024 * 1024 * 5
  },
  fileFilter: fileFilter
})

async function DoUploadImage(request, response) {
    const userId = emailtoken.CheckToken(emailtoken)
    if (userId != null) {
        response.status(200).json({
            success: true
        })
    } else {
        response.status(200).json({
            success: false
        })
    }
}

router.post("", upload.single("uploadImage"), (request, response, next) => {
    DoUploadImage(request, response)
})

module.exports = router

