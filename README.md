# Reborn_2022
Picking back where we left off in 2021 with unreal engine c++/blueprints coding/learning\
![image](https://jacobmottgithub.s3.amazonaws.com/Reborn2022/Screenshots/Reborn_2022.png)


Working through Dev Enabled youtube channels UE4 C++ playlist


https://www.youtube.com/channel/UCL1jcpKRApOp_CkW9LkFRKA

https://www.youtube.com/watch?v=b7qC1ogIFpk&list=PL9z3tc0RL6Z4JJS__Bge8O2mLwKUaG1eU


Using UWidgets in 3d space(not on screen) for health, and showing things like names and information in game(sessions available, client or host)

Added multiplayer lobby, you can "Host" or join/find sessions

Mutliplayer lobby was done by using this tutorial and tweaking it to my needs.
https://cedric-neukirchen.net/2021/06/27/ue4-multiplayer-sessions-in-c/




## AWS S3 Screenshots And videos folders (Syncing/Pull/Push to)

<details>
The Content folder is stored in s3 bucket

Pull down from bucket
  
```
  aws s3 cp --recursive s3://<bucket>/Reborn2022/Content Reborn2022/Content  
  aws s3 cp --recursive s3://<bucket>/Reborn2022/Screenshots Reborn2022/Screenshots    
```

Push to bucket
```
  aws s3 cp --recursive Reborn2022/Content s3://<bucket>/Reborn2022/Content
  aws s3 cp --recursive Reborn2022/Screenshots s3://<bucket>/Reborn2022/Screenshots
```

Or just do a sync
```
  aws s3 sync Reborn2022/Content s3://<bucket>/Reborn2022/Content --delete  
  aws s3 sync Reborn2022/Screenshots s3://<bucket>/Reborn2022/Screenshots --delete  

```
</details>
