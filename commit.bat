@echo off
git add *
git status
SET /P "msg=Commit message:"
SET /P "branch=Branch to commit to:"
SET /P "rdy=Ready to commit [y/n]?:"
IF "%rdy%"=="y" (
    git commit -m "%msg%"
    git push origin %branch%
)
PAUSE