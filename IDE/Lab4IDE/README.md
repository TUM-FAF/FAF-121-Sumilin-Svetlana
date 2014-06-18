Laboratory Work №4
==================
Web development 

First I Installed VMware Workstation for Ubuntu.
Installed rails: $ gem install rails

Created my new Application: ```  $ rails new blog ``` 
Made my Application server running: ``` $ rails server``` 

I was checking my progress by navigating to: ```  http://localhost:3000.``` 

To make a hello page I used:  ```  $ rails generate controller welcome index``` sh
and in index.html.erb file I have written my name.

Then I made it able to create some articles 
``` sh
class ArticlesController < ApplicationController
  def new
  end
 
  def create
  end
end
``` 

And changed the article model:
``` sh
$ rails generate model Article title:string text:text
``` 
And made it to be possible to save the articles
``` sh
@article.save
  redirect_to @article
end
``` 
