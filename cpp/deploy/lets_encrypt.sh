if [ "$#" -ne 1 ]; then
    echo "Please provide a domain name as argument."
    echo "Usage:"
		echo "  deploy/lets_encrypt.sh deeprevelations.com"
		exit 1
fi

DOMAIN_NAME=$1

echo "You are about to install 'Let's Encrypt' for the the domain [${DOMAIN_NAME}]."
echo "Press Ctrl-C to abort."
read -r -p "" response

sudo add-apt-repository ppa:certbot/certbot
sudo apt install python-certbot-nginx
sudo certbot --nginx -d ${DOMAIN_NAME}
