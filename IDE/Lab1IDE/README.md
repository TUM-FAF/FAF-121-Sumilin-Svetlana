Laboratory Work №1
==================

The purpose of this laboratory work was to learn Command Line Interface; CLI Editors; Setting Server Environment; Version Control Systems


Connect to a remote server via SSH
SSH – secure shell, a command interface for getting access to a remote computed secured. I connected using GitBash, run ssh usernme@IP line to connect.
Tried commands like: mkdir – that created a directory, ls- to see all files, cd\cd dir – change the directory, rm file- delete files, touch file- to create.
Initialize a repository on server
I used git init command that created a new repository on Git. That is the first command to run for a first project.
Create a file in repository folder, write in your name, save it and commit it
mkdir Sumilin   		# folder created
vim file.txt   			# create a txt file              
i              			# insert needed text              
Svetlana Sumilin		# text written
:wq                    	# write the text and exit       
git add file.txt   		# add just this .txt to folder        
git commit -m "Blah Blah Blah"# comments when commited


Tasks With Points
Create your own server (ex. virtual machine)
Used VMware Workstation for Ubuntu.
sudo apt-get install openssh-server  	#install OpenSSH    
sudo service ssh start 		#start  server
sudo service ssh stop			#stop server


Connect to server using public key

Create 2 more branches with at least one unique committed file per branch
git add <file> - stage all changes, and in next commit they will be seen.
git commit – commits to the “history”. Files won’t change unless we ask it (sometimes we use a commit message).
git branch – list all branches in repository
git branch <name> - create a new branch
git branch –d <name> - delete this branch
git checkout <name> - choose the current branch to be <name>
git branch first		# create one branch
git branch second		# create second branch
git checkout first		# move to first branch
git touch file.txt		# create new file
git add file.txt		# add this file
git commit –m ”first”		# commit with comment
git checkout second		# move to second branch
git touch file2.txt		# create new file
git add file2.txt		# add this file
git commit –m “second”		# commit with comment



Set a branch to track a remote origin on which you are able to push (ex. github, bitbucket or a custom server)

git remote add origin <url> #add new remote, now we use origin instead of link 
git push –u origin master   #push the commits in the local branch named master to the remote named origin

-u - means that for every branch that is up to date or successfully pushed, add tracking reference. It should be done just for first push to repository.


Reset a branch to previous commit, reset a branch to some specific commit

git reset <file>     # remove specific file 

git reset --hard ID # ID for example is 8ff42d

It is considered a dangerous command because it throws away all uncommitted changes.

git reset --hard HEAD~2 # HEAD points to current branch, rewind HEAD to get rid of 2 last commits

It is used to throw uncommitted changes.



Restore a reset branch back to its previous state

git reflog			#shows history
git checkout <branch>		#go to the branch
git reset --hard 11bbf2 	#do a reset to the needed commit  

git reflog  - works just for a certain amount of time because Git cleans reflog sometimes. It retrieves a list of commits and actions done.

    	 
GIT cherry-pick, rebase

git checkout -b third 			#move to branch
touch file3.txt				#create file
git add file3.txt			#add file to branch
git commit -a -m "third"		#commit and comment
git checkout second			#go to second branch
git rebase first      			# first branch moves to second
git cherry-pick third			# apply the commit from third to the current branch		

When we lose something or destroy the wrong commit, we can restore it using git cherry-pick.
Difference between the rebase and cherry-pick: cherry-pick takes just one commit (sometimes a few of them) and move to a new location. Rebase take all changes on a branch and replay it to another branch.


Make your CLI text editor to highlight code

First I opened a .txt file: vim sveta.txt
Then I typed some text (code). Pressed Esc and wrote :syntax on


Create a VCS alias

These two are some good shortcuts:
git config --global alias.br branch	#create branch
git config --global alias.co checkout #move between branches
