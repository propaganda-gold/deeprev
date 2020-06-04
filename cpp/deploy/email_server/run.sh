#!/bin/bash
sudo /prod/email_server --http_options=/secrets/deeprevelations.http_options.prod.pbtxt --sendgrid_secret_file=/secrets/deeprevelations-at-sendgrid.auth-key
