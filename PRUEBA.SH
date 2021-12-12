#Soy un comentario
Mkdisk  -path~:~/home/user/Disco2.disk -Unit~:~K -size~:~3000
Mkdisk  -path~:~/Disco2.disk -Unit~:~K -size~:~3000
#Mkdisk -Size~:~3000 –unit~:~K -path~:~/home/user/Disco1.disk

Mkdisk -size~:~4 -unit~:~k -path~:~/home/a/disco.disk #Soy un comentario al final de la linea
pAuSe
mkDisk -size~:~100 -unit~:~m -path~:~/a/discoda.disk
pause #SOY COMENTARIO
rmdisk -path~:~/home/user/Disco2.disk
pause
rmdisk -path~:~/home/a/disco.disk
rmdisk -path~:~/home/user/Disco2.disk


#Disco y particion pirmaria
Mkdisk  -path~:~/Disco3.disk -Unit~:~K -size~:~3000
Mkdisk  -path~:~/Disco2.disk -Unit~:~K -size~:~3000
fdisk -path~:~/Disco3.disk -size~:~500 -unit~:~k -name~:~Part1P
fdisk -path~:~/Disco2.disk -size~:~500 -unit~:~k -name~:~Part1P
fdisk -path~:~/Disco3.disk -size~:~500 -unit~:~k -type~:~E -name~:~Part1E

mount -path~:~/Disco3.disk -name~:~Part1P
mount -path~:~/Disco2.disk -name~:~Part1P
mount -path~:~/Disco3.disk -name~:~Part1E
unMount -id~:~vda1
unMount -id~:~vda2

fdisk -path~:~/Disco3.disk -size~:~250 -unit~:~k -type~:~L -name~:~Part1L 
rmdisk -path~:~/home/user/Disco2.disk
mkfs -id~:~vda1 -fs~:~2fs

login -usr~:~root -pwd~:~123 -id~:~vda1