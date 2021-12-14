$(document).ready(function () {
    session = new QiSession();

    session.service("ALMemory").done(function (ALMemory) {

        ALMemory.subscriber("JsonHomeList").done(function (subscriber) {

            subscriber.signal.connect(function (data) {
                var html = '<h1>Choix du home</h1>';
                const homes = jQuery.parseJSON(data);

                $.each(homes, function (key, home) {
                    html += '<hr><div>';
                    if (home.label !== 'undefined' && home.id !== 'undefined') {
                        html += '<button class="btn_choice_home" type="button" data-id="' + home.id + '">' + home.label + '</button>';
                    }
                    html += '</div><hr>';
                });

                $('.home').html('');
                $('.choice_home').html(html);
            });
        });

        ALMemory.subscriber("JsonHome").done(function (subscriber) {

            subscriber.signal.connect(function (data) {
                var html = '';
                const home = jQuery.parseJSON(data)

                if (home.label !== 'undefined') {
                    html += '<h1>Home: ' + home.label + '</h1>';
                }

                if (home.rooms !== 'undefined') {
                    html += '<div><h2>Liste des Rooms</h2><div>';

                    var rooms = '';
                    $.each(home.rooms, function (key, room) {
                        rooms += '<hr><div>';
                        if (room.label !== 'undefined') {
                            rooms += '<p>Room: ' + room.label + '</p>';
                        }

                        var elements = '<h3>Liste des elements</h3>';
                        if (room.elements !== 'undefined') {
                            elements += '<ul>';
                            $.each(room.elements, function (key, element) {
                                elements += '<li>';
                                if (element.label !== 'undefined') {
                                    elements += '<p>Element: ' + element.label + '</p>';
                                }
                                if (element.elementValue !== 'undefined' && element.elementValue !== null && element.elementValue.value !== 'undefined' && element.elementValue.datetime !== 'undefined') {
                                    var dateTime = element.elementValue.datetime;

                                    try {
                                        var dateFormat = element.elementValue.datetime.replace('T', ' ').replace(/-/g, "/").split('+')[0];
                                        var dateObject = new Date(dateFormat);

                                        var date = dateObject.getDate() + '/' + (dateObject.getMonth() + 1) + '/' + dateObject.getFullYear();

                                        var time = dateObject.getHours() + ":" + dateObject.getMinutes() + ":" + dateObject.getSeconds();

                                        dateTime = date + ' à ' + time;
                                    } catch (error) { }

                                    elements += '<p>Value: ' + element.elementValue.value + ' le ' + dateTime + '</p>';
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

                    html += rooms;

                    html += '</div></div>';
                }

                $('.home').html(html);
                $('.choice_home').html('<button class="btn_reset_home" type="button"">Choisir un autre home</button>');
            });
        });

    });

    function raise(event, value) {
        session.service("ALMemory").done(function (ALMemory) {
            ALMemory.raiseEvent(event, value);
        });
    }

    $(document).on('click', '.btn_choice_home', function () {
        var home_name = $(this).data('id');
        raise('ChoiceHome', home_name);
    });

    $(document).on('click', '.btn_reset_home', function () {
        raise('ResetHome', 1);
    });

});
