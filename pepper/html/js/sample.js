$(document).ready(function () {
    session = new QiSession();

    session.service("ALMemory").done(function (ALMemory) {

        ALMemory.subscriber("JsonHome").done(function (subscriber) {

            subscriber.signal.connect(function (data) {
                const home = jQuery.parseJSON(data)

                if (home.label !== 'undefined') {
                    $('.home').html(home.label);
                }

                if (home.rooms !== 'undefined') {
                    var rooms = '';
                    $.each(home.rooms, function(key,room) {
                        if (room.label !== 'undefined') {
                            rooms += '<hr><div><p>' + room.label + '</p></div><hr>';
                        }
                    });

                    $('.rooms').html(rooms);
                }
            });
        });

    });

    function raise(event, value) {
        session.service("ALMemory").done(function (ALMemory) {
            ALMemory.raiseEvent(event, value);
        });
    }

});
