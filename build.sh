PROJ_DIR=`pwd`
cd /home/laginimaineb/Android/code/android-4.4
TOP=`pwd`
export TOP
source build/envsetup.sh
cd $PROJ_DIR
mm
cp $TOP/out/target/product/generic/system/bin/aps .
