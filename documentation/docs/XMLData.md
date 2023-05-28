# Table "XMLData"

???+ info
    A public table containing all the functions related to gathering XML attributes accross the different xmls with updated values to match the real values.
        
## Functions

### Get·Entity·By·Name () {: aria-label='Functions' }
#### table GetEntityByName ( string name ) {: .copyable aria-label='Functions' }
Returns a table containing the attributes of the entities2.xml that match the given name attribute. All of the attributes are lowercase for ease of access.
???- info "Table usage"
	```lua
	print("Monstro's type:", XMLData.GetEntityByName("Monstro").type)
	```
___

### Get·Mod·By·Name () {: aria-label='Functions' }
#### table GetModByName ( string name ) {: .copyable aria-label='Functions' }
Returns a table containing the attributes of the metadata.xml that match the given name attribute. All of the attributes are lowercase for ease of access.
???- info "Table usage"
	```lua
	print("Car mod's version:", XMLData.GetModByName("Car").version)
	```
___

### Get·Music·By·Name () {: aria-label='Functions' }
#### table GetMusicByName ( string name ) {: .copyable aria-label='Functions' }
Returns a table containing the attributes of the music.xml that match the given name attribute. All of the attributes are lowercase for ease of access.
???- info "Table usage"
	```lua
	print("Basement track loop:", XMLData.GetMusicByName("Basement").loop)
	```
___

### Get·Player·By·Name () {: aria-label='Functions' }
#### table GetPlayerByName ( string name ) {: .copyable aria-label='Functions' }
Returns a table containing the attributes of the players.xml that match the given name attribute. All of the attributes are lowercase for ease of access.
???- info "Table usage"
	```lua
	print("Isaac's bomb number:", XMLData.GetPlayerByName("Isaac").bombs)
	```
___
