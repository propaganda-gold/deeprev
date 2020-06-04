const objects = require("../models/objects")
const resources = require("../util/resources")
const mongoose = require("mongoose")
const cheerio = require('cheerio');
const getUrls = require('get-urls');
const fetch = require('node-fetch');

async function CreateLabel(subjectId, objectId, objectType, labelName, labelValue) {
    console.log('CreateLabel', subjectId, objectId, objectType, labelName, labelValue)
    const creationTime = 0
    const label = new objects.Label({
        _id: new mongoose.Types.ObjectId(),
        subjectId,
        objectId,
        objectType,
        labelName,
        labelValue,
        creationTime,
    })

    return label.save()
}

module.exports = {
    CreateLabel,
}