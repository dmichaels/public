## My .bash_profile for Mac OS.
## February 2017
## dmichaels
##

# Java environment variable(s).
#
#export JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_102.jdk/Contents/Home
export JAVA_HOME=/Users/dmichaels/home/software/jdk-11.0.10.jdk/Contents/Home

# Maven environment variable(s).
#
#export MAVEN_HOME=${HOME}/home/dev/software/maven
#export MAVEN_HOME=${HOME}/home/dev/thirdparty/apache-maven-3.6.3
export MAVEN_HOME=$HOME/home/software/apache-maven-3.6.3
export M2=${HOME}/.m2

# Mule environment variable(s).
#
export MULE_HOME=/Users/dmichaels/home/dev/software/mule-standalone-3.8.1

# Cartera Marketing Platform (aka MP, aka MAPL) environment variable(s).
#
export MP_ENV=dmichaels
export MP_HOME=/Users/dmichaels/home/dev/mapl/git-projects/mp-api

# Bash environment variable(s).
# Unlimited shell command history.
#
export HISTSIZE=
export HISTFILESIZE=

# Vertica command-line client environment variable(s).
#
export VERTICA_HOME=${HOME}/home/dev/software/vertica/opt/vertica

# Vagrant environment variable(s).
#
export VAGRANT_HOME=${HOME}/home/dev/software/vagrant

# Node.js environment variable(s).
#
# export NODE_HOME=${HOME}/home/dev/software/node
# export NODE_PATH=${NODE_HOME}/lib/node_modules
# source ${NODE_PATH}/npm/lib/utils/completion.sh
#
# export NODE_PATH=/usr/local/lib/node_modules
# source ${NODE_PATH}/npm/lib/utils/completion.sh

# General path environment variable.
#
# N.B. The default PATH directories on Mac OS X are in /etc/paths - these:
#
#      /usr/local/bin
#      /usr/bin
#      /bin
#      /usr/sbin
#      /sbin
#
# So no need to duplicate them here/below.
#
# For RabbitMQ, at least - perhaps this is standard for
# other installed software - we need /usr/local/sbin in ouf PATH.
# To have RabbitMQ start automatically on startup:
#    ln -sfv /usr/local/opt/rabbitmq/*.plist  ~/Library/LaunchAgents
# Otherwise, to start the RabbitMQ server:
#    rabbit-server &
# Then browse to: http://localhost:15672
# The default username and password is guest and guest.
#
export PATH=${PATH}:${JAVA_HOME}/bin:${MAVEN_HOME}/bin:/usr/local/sbin:${VERTICA_HOME}/bin:${HOME}/home/bin:.

# Command-line prompt.
#
export PS1='lo: '

# Forgot what this is for iTerm2 shell integration stuff is exactly for ...
#
# test -e "${HOME}/.iterm2_shell_integration.bash" && source "${HOME}/.iterm2_shell_integration.bash"

# Aliases.
#
alias cm='chmod'
alias cmx='chmod a+x'
#alias ctags="`brew --prefix`/bin/ctags"
alias d='dirs'
alias f='fgrep'
alias ff='fgrep -i'
alias fg1='fg %1'
alias fg2='fg %2'
alias fg3='fg %3'
alias fg4='fg %4'
alias fg5='fg %5'
alias fg6='fg %6'
alias fg7='fg %7'
alias ftpeti='sftp 117005@ftp1.exacttarget.com:import'
alias gb='git branch'
alias gbm='git branch -a | more'
alias gba='git branch -a'
alias gca='git commit -a -m'
alias grem='git remote -v'
alias gd='git diff'
alias gco='git checkout'
alias gcom='git checkout master'
alias gpa='git pull --all'
alias gitpush='git push'
alias grev='git checkout HEAD --'
alias saveconf="cp -pR ~/.ssh ~/.gitconfig ~/.git-credentials ~/.bash_profile ~/.vimrc ~/.vim ~/home/etc/conf ; cp -p ~/Library/Preferences/com.googlecode.iterm2.plist ~/home/etc/conf/iterm2"
alias gupetc='(cd ~/home/etc ; gca etc ; git push ; gpa ; gs)'
alias gupetca="cp -pR ~/.ssh ~/.bash_profile ~/.vimrc ~/.vim ~/home/etc/conf/home ; cp -p ~/Library/Preferences/com.googlecode.iterm2.plist ~/home/etc/conf/iterm2 ; gupetc"
alias gupetcan="(cd ~/home/etc/notes/.archive ; zip -e notes.archive.zip notes.*) ; gupetca"
alias gs='git status -s'
alias gsm='git status -s | more'
alias h='history'
alias hf='history |fgrep -i'
alias httprestart='sudo apachectl stop ; sleep 1 ; sudo apachectl start'
alias j='jobs'
alias l='ls -laF'
alias lh='ls -laFh'
alias la='ls -laF'
alias lah='ls -laFh'
alias lf='ls -aF'
alias lt='ls -laFt'
alias lth='ls -laFth'
alias ltm='ls -laFt | more'
alias ltmh='ls -laFth | more'
alias lz='ls -laFS'
alias lzh='ls -laFSh'
alias m='more'
alias md='mkdir'
alias mcpi='mvn clean package install'
alias muleq='ps -ef | fgrep mule'
alias muler='${MULE_HOME}/bin/mule stop ; ${MULE_HOME}/bin/mule start'
alias mulestop='${MULE_HOME}/bin/mule stop'
alias mulestart='${MULE_HOME}/bin/mule start'
alias no='vi ~/home/etc/notes/notes.cartera'
alias nom='vi ~/home/etc/notes/notes.mac'
alias pd='pushd'
alias pd2='pushd +2'
alias pd3='pushd +3'
alias pdd='pushd +2'
alias pddd='pushd +3'

alias pddev='pushd ${HOME}/home/dev'
alias pdetc='pushd ${HOME}/home/etc'
alias pdpub='pushd ${HOME}/home/public'
alias pdhypr='pd ~/home/dev/hypr'

alias dev='cd ~/home/dev'
alias etc='cd ~/home/etc'
alias pub='cd ~/home/public'
alias hypr='cd ~/home/dev/hypr'

alias pdmp='pushd ${HOME}/home/dev/mapl/git-projects/mp-api'
alias pdvagrant='pushd ${VAGRANT_HOME}'
alias pdmule='pushd ${MULE_HOME}'
alias pdmule35='pushd /Users/dmichaels/home/dev/software/mule-standalone-3.5.0'
alias pdmule38='pushd /Users/dmichaels/home/dev/software/mule-standalone-3.8.0'
alias pdnotes='pushd ${HOME}/home/etc/notes'
alias pp='popd'
alias pp2='popd +2'
alias pp3='popd +3'
alias ppp='popd +2'
alias pppp='popd +3'
alias rd='rmdir'
alias rm='rm -i'
alias rmqstart='rabbitmq-server & echo "RabbitMQ Console: http://localhost:15672"'
alias up='cd ..'
alias upp='cd ../..'
alias uppp='cd ../../..'
alias upppp='cd ../../../..'
alias uppppp='cd ../../../../..'
alias '..'='cd .. ; pwd'
alias '...'='cd ../.. ; pwd'
alias '....'='cd ../../.. ; pwd'
alias '.....'='cd ../../.. ; pwd'
alias '......'='cd ../../../../.. ; pwd'
alias '.......'='cd ../../../../../.. ; pwd'
alias '........'='cd ../../../../../../.. ; pwd'
alias '.........'='cd ../../../../../../../.. ; pwd'
alias '..........'='cd ../../../../../../../../.. ; pwd'
alias '...........'='cd ../../../../../../../../../.. ; pwd'
alias '............'='cd ../../../../../../../../../../.. ; pwd'
alias vibash='vim ~/.bash_profile ; source ~/.bash_profile'
alias vivi='vim ~/.vimrc'

# Git repo/branch info in title bar and or whatnot.
# Commented out because too slow. Git branch can be displayed in VIM airline.
#
#function iterm2_print_user_vars() {
#  iterm2_set_user_var gitBranch $((git branch 2> /dev/null) | grep \* | cut -c3-)
#  iterm2_set_user_var gitRepo $(basename `git rev-parse --show-toplevel`)
#}
#
#function iterm2_print_user_vars() {
#  iterm2_set_user_var gitBranch $((git branch 2> /dev/null) | grep \* | cut -c3-)
#  iterm2_set_user_var gitRepo $((git remote show origin 2> /dev/null) | grep "Fetch URL:" | cut -c14-)
#}
#
#parse_git_branch() {
#  git branch 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/ (\1)/'
#}
#
#get_git_root() {
#  basename $(git rev-parse --show-toplevel 2> /dev/null) 2> /dev/null
#}
#
#update_git_prompt()
#{
#   GIT_BRANCH=$(parse_git_branch)
#
#   if [ -n "$GIT_BRANCH" ]; then
#       GIT_ROOT=$(get_git_root)
#       echo -ne "\033]0;$(get_git_root): $(parse_git_branch)\007"
#   else
#       echo -ne "\033]0;\007"
#   fi
#}
#
#PROMPT_COMMAND="update_git_prompt; $PROMPT_COMMAND"

cd ~/home/etc/notes/my
alias s=swift
alias swiftinit='swift package init --type=executable'

alias rmdss='rm -f `find . -type f -name .DS_Store`'
alias lsdss='find . -type f -name .DS_Store'
alias jso='python -m json.tool'
alias notes='cd ~/home/etc/notes/my'
alias dush='du -s -h'
alias ma='make'
alias mk='make'
alias mke='make'
alias mak='make'
alias mka='make'
alias amke='make'
alias mkae='make'
alias mkea='make'

#ssh-add ~/.ssh/amazon-aws-private-key.pem
ssh-add ~/.ssh/amazon-aws-private-key-20210307.pem # instance: i-0e4d076f94df75b8c

#alias sshaws='ssh ubuntu@52.43.145.141'
#alias sshaws='ssh ubuntu@52.12.192.146'
#alias sshaws='ssh ubuntu@54.184.21.34'
#alias sshaws='ssh ubuntu@34.217.49.72'
#alias sshaws='ssh ubuntu@34.214.208.221'
#alias sshaws='ssh ubuntu@54.149.38.35'
#alias sshaws='ssh ubuntu@54.213.127.178'
#alias sshaws='ssh ubuntu@54.187.96.158 '
#alias sshaws='ssh ubuntu@34.215.247.15'
#alias sshaws='ssh ubuntu@34.220.246.72'
#alias sshaws='ssh ubuntu@54.244.171.6'
#alias sshaws='ssh ubuntu@54.191.130.193'
#alias sshaws='ssh ubuntu@34.208.212.121'
#alias sshaws='ssh ubuntu@34.219.224.227'
#alias sshaws='ssh ubuntu@54.149.102.167'
alias sshaws='ssh ubuntu@34.217.83.239'

#export CMAKE_PATH=~dmichaels/home/dev/cmake-3.20.0-rc4-macos-universal/CMake.app/Contents
 export CMAKE_PATH=~dmichaels/home/dev/cmake-3.20.0-rc5-macos-universal/CMake.app/Contents
 export PATH=$PATH:$CMAKE_PATH/bin
