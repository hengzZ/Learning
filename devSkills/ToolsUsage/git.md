## Git

##### 1. 查看本地修改
```
$ git checkout
$ git status
$ git diff [文件名]
```

##### 2. 提交修改并添加 commit
```
$ git add [文件名 或 文件夹]
$ git commit  // Ctrl+字符，保存并退出commit编辑界面(ctrl+o,回车,退出)
$ git push origin master
```

##### 3. git pull 强制覆盖本地修改
```
$ git fetch --all
$ git reset --hard origin/master
$ git pull
```

##### 4. 取消 git add 的添加项
```
$ git reset HEAD // HEAD 后面为空代表取消所有 add 内容
$ git reset HEAD XXX/XXX/XXX.java // 对指定文件撤销 add 操作
```

##### 5. 修改 git commit 编辑器为 vim
```
$ vim ~/.gitconfig
[core]
	editor = vim
[user]
	email = xxxxxxxx
	name = hengzZ
```

##### 6. 修改 commit 内容
```
$ git commit -amend
```

##### 7. review patch 提交
```
	TODO
```

##### 8. 查看提交记录
```
$ git log  //查看提交的历史版本信息
```

##### 9. 回退到某次修改状态
```
$ git reset --soft [git log 内commit码]   //不撤销添加的修改内容
$ git reset --hard  [git log 内commit码]  //撤销所有修改内容，还原为指定版本
```

##### 10. Git 保存当前工程的快照
```
$ git stash list                    # 显示列表
$ git stash apply [快照]             # 应用最新的快照，不从列表中删除
$ git stash pop [快照]               #  应用最新的快照，并从列表中删除
$ git stash save -a "message"        # 保存当前进度快照，并添加备注说明信息
$ git stash drop [快照]              # 删除指定快照
```

##### 11. ag 关键词所在行查询
```
$ ag [options] pattern [path]
```

##### 12. 多 branch 间代码切换
```
$ git branch
$ git status
$ git branch [branch name] // 创建一个新分支
$ git checkout [branch name] // 切换到新分支
$ git checkout -b [branch name] // 创建一个新分支，并切换到创建的分支中
```

##### 13. 查看远程仓库信息
```
$ git remote -v // 查看仓库地址
$ git branch -a // 查看分支列表，以及当前所属分支
```

##### 14. 查看远程更新信息（更新 commit 信息）
```
$ git fetch // 效果：get pull = git fetch + git merge
```

##### 15. 提交代码到一个新的分支
```
$ git branch [branch name]  # 创建一个新的本地分支
$ git push origin [branch name]  # 推到远端分支
```

##### 16. 从 Github 安装 python 包
```
pip install git+https://github.com/wkentaro/labelme.git
```

##### 17. 创建、删除分支
```
在本地新建一个分支： git branch [newBranch]
将新分支发布在 github 上： git push origin [newBranch]
在本地删除一个分支： git branch -d newBranch
在 github 远程端删除一个分支： git push origin :newBranch   (分支名前的冒号代表删除)
```

##### 18. 分支合并
```
git checkout master
git merge [newBranch]  (将 newBranch 合并入 master 分支)
```

##### 19. 解决： 文件超过 100 MB，导致 git push error.
```
1. 移除错误缓存 （例如：文件 libbaiduNaviSDK.a）
git rm --cached -r sr/libbaiduNaviSDK.a
2. 重新提交
git commit --amend
git push

## 针对必须要上传的大文件 （请先从工程移出它们，然后使用 Git Large File Storage 统一上传！）
1. 首先安装 git-lfs
brew install git-lfs
2. 待上传对象添加进 track：
git lfs track "libbaiduNaviSDK.a"
3. 将想要保存的大文件正常添加进 git：
git add sr/libbaiduNaviSDK.a
4. 正常进行提交 & 推送
git commit -m "Add design file"
git push origin master
```
