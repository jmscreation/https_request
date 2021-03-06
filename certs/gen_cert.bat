@echo off
echo Generate Key...
openssl genrsa -out server.key 4096

echo Generate Server Certificate Request...
openssl req -new -key server.key -sha256 -nodes -out server.csr -config serverconfig.cnf

echo Signing Certificate With CA...
openssl x509 -req -sha256 -CA CA.pem -CAkey CA.key -CAcreateserial -CAserial CA.srl -in server.csr -days 400 -extensions server -extfile serverconfig.cnf -out server.pem