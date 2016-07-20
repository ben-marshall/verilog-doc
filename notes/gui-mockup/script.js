
/*
Contains all of the functions for the documentation pages.
*/

/*
Responsible for taking a JSON data structure and turning it into a list
on the list.html page.
*/
function veridoc_render_list(
    data
){
    var listType = data.listType;
    var listData = data.listData;
    var listTitle = data.listTitle;
    var listNotes = data.listNotes;

    document.getElementById('list-title').innerText = listTitle;
    document.getElementById('list-notes').innerText = listNotes;

    // This is a <ul> element
    var container = document.getElementById('list-container');
    container.innerHTML = "";

    var toset = ""

    listData = listData.sort(function(a,b){
        if(a.path>=b.path){
            return 1;
        }
        else {
            return -1;
        }
    });

    var i;
    for(i = 0; i < listData.length; i++)
    {
        var item = listData[i];
        
        toset += "<li>";
        if(item.parsed == "1"){
            toset += "<div class='status good'>Found</div>";
        } else {
            toset += "<div class='status bad'>Not Found</div>";
        }
        if(item.success== "1"){
            toset += "<div class='status good'>Parsed</div>";
        } else {
            toset += "<div class='status bad'>Errors</div>";
        }
        
        toset += "<div class='item'>"+item.path+"</div>";
        toset += "</li>"

    }

    container.innerHTML = toset;
}
