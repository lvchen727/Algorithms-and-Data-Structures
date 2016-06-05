This lab is intended to give some practice with the skip list data type. We’ll use the skip list as a database indexing strategy that lets us make useful queries quickly. The specific application will be an interface to a good-sized database of historical events collected from several sources around the Web.

The basic skip list type can be made more space-efficient in two ways. I implement both of the following optimizations:
• Dynamically resize the head and tail pillars. • Singly-linked skip list 