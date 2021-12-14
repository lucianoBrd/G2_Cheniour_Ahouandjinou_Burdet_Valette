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
                        rooms += '<hr><div>';
                        if (room.label !== 'undefined') {
                            rooms += '<p>Room: ' + room.label + '</p>';
                        }

                        var elements = '<h3>Liste des elements</h3>';
                        if (room.elements !== 'undefined') {
                            elements += '<ul>';
                            $.each(room.elements, function(key,element) {
                                elements += '<li>';
                                if (element.label !== 'undefined') {
                                    elements += '<p>Element: ' + element.label + '</p>';
                                }
                                if (element.elementValue !== 'undefined' && element.elementValue !== null && element.elementValue.value !== 'undefined' && element.elementValue.datetime !== 'undefined') {
                                    var dateTime = element.elementValue.datetime;
                                    
                                    try {
                                        var dateFormat = element.elementValue.datetime.replace('T', ' ').replace(/-/g,"/").split('+')[0];
                                        var dateObject = new Date(dateFormat);

                                        var date = dateObject.getDate() + '/' + (dateObject.getMonth()+1) + '/' + dateObject.getFullYear();

                                        var time = dateObject.getHours() + ":" + dateObject.getMinutes() + ":" + dateObject.getSeconds();

                                        dateTime = date + ' à ' + time;
                                    } catch (error) {}
                                    
                                    elements += '<p>Value: ' + element.elementValue.value + ' le ' + dateTime +  '</p>';
                                }
                                if (element.type !== 'undefined' && element.type.label !== null) {
                                    elements += '<p>Type: ' + element.type.label + '</p>';
                                }
                                elements += '</li>';
                            });
                            elements += '</ul>';
                        }

                        rooms += elements;

                        rooms += '</div><hr>';
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
