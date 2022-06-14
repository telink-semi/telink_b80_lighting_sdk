# 删除list.txt文件
#rm list.txt

# 在源代码根目录，使用git status命令获取已修改文件的列表
git status | grep modified | awk '{print $2}' > list.txt
# 在当前目录下，创建temp目录
#mkdir temp
MK_DIRECTORY=diff_$(date +%y%m%d_%H_%M_%S)
mkdir $MK_DIRECTORY
# 将已修改文件列表逐一复制到创建的目录下
xargs -a ./list.txt cp --parents -t ./$MK_DIRECTORY

# 将temp目录下的所有文件打包为modules.tar.gz

#cd temp
cd $MK_DIRECTORY
#tar -czf modules.tar.gz *
mv ../list.txt  ./