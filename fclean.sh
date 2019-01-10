if [ -f Makefile ]
	then sudo make clean
fi
sudo rm -f app/Makefile
sudo rm -f model/Makefile
sudo rm -f model/libmodel.a
sudo rm -f view/Makefile
sudo rm -f view/libview.a
sudo rm -f Makefile
sudo rm -f ctop

