
/*
Contains all of the functions for the documentation pages.
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
}
