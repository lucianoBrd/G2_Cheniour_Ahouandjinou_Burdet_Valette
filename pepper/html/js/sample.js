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
                                if (element.type !== 'undefined' && element.type.label !== null && element.label !== 'undefined') {
                                    var type = element.type.label;
                                    var action = null;
                                    var state = null;

                                    elements += '<p>Type: ' + type + '</p>';

                                    elements += '<div>Action: ';
                                    if (element.action !== 'undefined' && element.action !== null && element.action.value !== 'undefined' && element.action.state !== 'undefined') {
                                        action = element.action.value;
                                        state = element.action.state;
                                    }

                                    if ((type === 'sensor' || type === 'actuator') && state !== null) {
                                        elements += '<label for="' + room.id + '-' + element.id + '">Etat: ' + (state ? 'Done' : 'DOING') + '</label>';
                                    }

                                    var found = false;
                                    if (type === 'sensor') {
                                        found = true;
                                        elements += '<input class="input_change_action" id="' + room.id + '-' + element.id + '" data-element="' + element.label + '" type="number" ' + (action ? ('value="' + action + '">') : '>');
                                    } else if (type === 'actuator') {
                                        found = true;
                                        elements += '<select class="input_change_action" id="' + room.id + '-' + element.id + '" data-element="' + element.label + '"><option value="">Choisir</option><option ' + ((action && action === 'ON') ? 'selected' : '') + ' value="ON">ON</option><option ' + ((action && action === 'OFF') ? 'selected' : '') + ' value="OFF">OFF</option></select>';
                                    }

                                    if (found) {
                                        elements += '<button data-input="' + room.id + '-' + element.id + '" class="button_change_action" type="button" class="btn btn-primary">Valider</button>';
                                    }

                                    elements += '</div>';
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
                $('.choice_home').html('<button class="btn_reset_home" type="button"">Choisir un autre home</button><small>(ou dire reset)</small>');
            });
        });

    });

    function raise(event, value) {
        session.service("ALMemory").done(function (ALMemory) {
            ALMemory.raiseEvent(event, value);
        });
    }

    function say(value) {
        session.service('ALTextToSpeech').done(function (tts) {
            tts.say(value);
        });
    }

    $(document).on('click', '.btn_choice_home', function () {
        var home_id = $(this).data('id');
        var label = $(this).html();

        raise('ChoiceHome', home_id);
        //say(label);
    });

    $(document).on('click', '.button_change_action', function () {
        var input = $('#' + $(this).data('input'));
        var element_name = $(input).data('element');
        var value = $(input).val();

        var parameter = '{"elementName": "' + element_name + '", "value": "' + value + '"}';

        raise('CreateAction', parameter);
    });

    $(document).on('click', '.btn_reset_home', function () {
        raise('ResetHome', 1);
    });

});
