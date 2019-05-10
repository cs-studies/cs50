$(function() {
    $('#form-register').on('click', 'button[type=submit]', function(event) {
        event.preventDefault();
        event.stopPropagation();
        let username = $('input[name=username]').val();
        if (!username) {
            return;
        }
        $.ajax({
            method: 'GET',
            url: "/check",
            data: {username: username},
            dataType: 'json'
        })
        .done(function(valid){
            if (valid === true) {
                $('form').submit();
            } else {
                alert("Username " + username + " is not available.");
            }
        })
    })
});