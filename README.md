# RSA
Cifrado RSA por bloques

### Prerequisitos

NTL (Number Theory Library)

```
sudo pacman -S ntl
```
### Compilar y Ejecutar
```
g++ rsa.cpp -o rsa -I ../include -L ../lib -lntl -lm
./rsa
```
