# SyncMarkGenerator
## Generating Sync Markers, using Animation Modifiers  
When working with animation assets in Unreal we can utilise Animation Modifiers to avoid repeated, manual work.  
We can create and execute logic for creating Curves, adding any type of Notifies for particles or sound or, like in this case, Sync Markers.  

## Step by step  
1. To open the modifiers tab, open your animation asset and select,   
Window/Animation Data Modifiers
![images](image1.png)
3. Search for the modifier you want to add, in our case the SyncMarkGenerator
4. Apply your settings  
    1. This will be the name of the Notify Track created  
    2. Make sure the entries here corresponds with the names of the bones in the rig  
    3. Select X, Y or Z depending on the forward axes, we want to use the forward axis to create our markers.  
    4. If you want to move all the markers forwards or backwards this setting can be used.  
5. Right click the modifier and select Apply Modifier to create the markers   	 

