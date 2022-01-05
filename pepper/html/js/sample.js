$(document).ready(function () {
    session = new QiSession();
    var src = null;

    session.service("ALMemory").done(function (ALMemory) {

        ALMemory.subscriber("JsonHomeList").done(function (subscriber) {

            subscriber.signal.connect(function (data) {
                var html = '';
                const homes = jQuery.parseJSON(data);

                src = null;

                html += '<div class="splash">';
                $.each(homes, function (key, home) {
                    if (home.hasOwnProperty('label') && home.hasOwnProperty('id') && home.label !== 'undefined' && home.id !== 'undefined') {
                        html += '<h1 class="splash-head btn_choice_home" data-id="' + home.id + '">' + home.label + '</h1>';
                    }
                });
                html += '</div>';

                $('.pure-menu-heading').html('Choix du home');
                $('.pure-menu-list').html('');
                $('.splash-container').html(html);
                $('.content-wrapper').html('');
            });
        });

        ALMemory.subscriber("AbsenceMode").done(function (subscriber) {

            subscriber.signal.connect(function (data) {
                var html = '';

                src = null;

                html += '<div class="splash">';
                html += '<h1 class="splash-head btn_goto_normal_mode">Retour mode normal</h1>';
                html += '</div>';

                $('.pure-menu-heading').html('Mode absence');
                $('.pure-menu-list').html('');
                $('.splash-container').html(html);
                $('.content-wrapper').html('<div class="content><div class="pure-g><form class="pure-form pure-form-stacked"><fieldset><label for="password">Mot de passe</label><input id="password" class="input_password" type="number"></fieldset></form></div></div>');
            });
        });

        ALMemory.subscriber("JsonHome").done(function (subscriber) {

            subscriber.signal.connect(function (data) {
                var html = '';
                var title = 'Home: NA';
                const home = jQuery.parseJSON(data);

                if (home.hasOwnProperty('cameraip') && home.cameraip !== 'undefined' && home.cameraip !== null) {
                    src = 'http://' + home.cameraip + '/capture';
                } else {
                    src = null;
                }

                if (home.hasOwnProperty('label') && home.label !== 'undefined') {
                    title = 'Home: ' + home.label;
                }

                if (home.hasOwnProperty('rooms') && home.rooms !== 'undefined') {

                    $.each(home.rooms, function (key, room) {
                        var issetRoom = false;
                        if ($('.room-' + room.id)[0]) {
                            issetRoom = true;
                        }

                        var rooms = '<div class="content room-' + room.id + '">';

                        if (room.hasOwnProperty('label') && room.label !== 'undefined') {
                            rooms += '<h2 class="content-head is-center room-label-' + room.id + '">Room: ' + room.label + '</h2>';
                        }

                        if (room.hasOwnProperty('elements') && room.elements !== 'undefined') {
                            var elementsContainer = '<div class="pure-g room-elements-' + room.id + '">';

                            $.each(room.elements, function (key, element) {
                                var issetElement = false;
                                if ($('.element-' + element.id)[0]) {
                                    issetElement = true;
                                }

                                var elements = '<div class="l-box pure-u-1-2 pure-u-md-1-2 pure-u-lg-1-4 element-' + element.id + '">';
                                if (element.hasOwnProperty('label') && element.label !== 'undefined') {
                                    elements += '<h4 class="element-label-' + element.id + '">Element: ' + element.label + '</h4>';
                                }

                                var stateLabel = (element.state ? 'ON' : 'OFF');
                                var value = 'NA';
                                var typeLabel = 'NA';

                                if (element.hasOwnProperty('elementValue') && element.elementValue !== 'undefined' && element.elementValue !== null && element.elementValue.hasOwnProperty('value') && element.elementValue.hasOwnProperty('datetime') && element.elementValue.value !== 'undefined' && element.elementValue.datetime !== 'undefined') {
                                    var dateTime = element.elementValue.datetime;

                                    try {
                                        var dateFormat = element.elementValue.datetime.replace('T', ' ').replace(/-/g, "/").split('+')[0];
                                        var dateObject = new Date(dateFormat);

                                        var date = dateObject.getDate() + '/' + (dateObject.getMonth() + 1) + '/' + dateObject.getFullYear();

                                        var time = dateObject.getHours() + ":" + dateObject.getMinutes() + ":" + dateObject.getSeconds();

                                        dateTime = date + ' à ' + time;
                                    } catch (error) { }

                                    value = element.elementValue.value + ' le ' + dateTime;
                                }

                                if (element.hasOwnProperty('type') && element.hasOwnProperty('label') && element.type !== 'undefined' && element.type.label !== null && element.label !== 'undefined') {
                                    var type = element.type.label;
                                    var action = null;
                                    var state = null;

                                    typeLabel = type;

                                    var actionForm = '<form class="pure-form pure-form-stacked"><fieldset>';
                                    var actionLabel = '<ul>';
                                    if (element.hasOwnProperty('action') && element.action !== 'undefined' && element.action !== null && element.action.hasOwnProperty('value') && element.action.hasOwnProperty('state') && element.action.value !== 'undefined' && element.action.state !== 'undefined') {
                                        action = element.action.value;
                                        state = element.action.state;
                                    }

                                    if ((type === 'sensor' || type === 'actuator') && state !== null) {
                                        actionLabel += '<li>Etat: ' + (state ? 'Done' : 'DOING') + '</li>';
                                    } else {
                                        actionLabel += '<li>Etat: NA</li>';
                                    }

                                    var found = false;
                                    if (type === 'sensor') {
                                        found = true;
                                        actionForm += '<input class="input_change_action" id="' + room.id + '-' + element.id + '" data-element="' + element.label + '" type="number" >';
                                    } else if (type === 'actuator') {
                                        found = true;
                                        actionForm += '<select class="input_change_action" id="' + room.id + '-' + element.id + '" data-element="' + element.label + '"><option value="">Choisir</option><option value="ON">ON</option><option value="OFF">OFF</option></select>';
                                    }

                                    actionLabel += '<li>Value: ' + (action ? action : 'NA') + '</li>';

                                    if (found) {
                                        actionForm += '<button data-input="' + room.id + '-' + element.id + '" class="button_change_action pure-button pure-button-primary" type="button" class="btn btn-primary">Valider</button>';
                                    }

                                    actionForm += '</fieldset></form>';
                                    actionLabel += '</ul>';
                                }

                                elements += '<table class="pure-table pure-table-bordered"><thead><tr><th>Etat Actuel</th><th>Value</th><th>Type</th><th>Action</th><th>#</th></tr></thead><tbody><tr><td class="element-state-' + element.id + '">' + stateLabel + '</td><td class="element-value-' + element.id + '">' + value + '</td><td class="element-type-' + element.id + '">' + typeLabel + '</td><td class="element-action-' + element.id + '">' + actionLabel + '</td><td>' + actionForm + '</td></tr></tbody></table>';

                                elements += '</div>';

                                elementsContainer += elements;

                                if (!issetElement) {
                                    $('.room-elements-' + room.id).append(elements);
                                } else {
                                    $('.element-label-' + element.id).html('Element: ' + element.label);
                                    $('.element-state-' + element.id).html(stateLabel);
                                    $('.element-value-' + element.id).html(value);
                                    $('.element-type-' + element.id).html(typeLabel);
                                    $('.element-action-' + element.id).html(actionLabel);
                                }
                            });
                            elementsContainer += '</div>';
                        }

                        rooms += elementsContainer;

                        rooms += '</div>';

                        html += rooms;

                        if (!issetRoom) {
                            $('.content-wrapper').append(rooms);
                        } else {
                            $('.room-label-' + room.id).html('Room: ' + room.label);
                        }
                    });

                }

                $('.pure-menu-heading').html(title);

                if (!$('.pure-menu-item')[0]) {
                    $('.pure-menu-list').html('<li class="pure-menu-item"><a href="#" class="pure-menu-link btn_reset_home">Reset</a></li><li class="pure-menu-item">(ou dire reset)</li>');
                }
                if (!$('.iframe_camera_home')[0]) {
                    $('.splash-container').html('<div class="splash"><p><iframe class="iframe_camera_home pure-img" src="" style="width: 100%;height: 200px;"></iframe></p><p class="splash-subhead">température climat temps "quelle temps fait-il"</p><p class="splash-subhead">humidité fraîcheur hygrométrie "quelle humidité fait-il"</p><p><a href="#" class="pure-button pure-button-primary btn_mode_absence">Mode absence</a></p></div>');
                }
                if (!$('.content')[0]) {
                    $('.content-wrapper').html(html);
                }
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

        if (value && value != '') {
            raise('CreateAction', parameter);
        }
    });
    
    $(document).on('click', '.btn_reset_home', function () {
        raise('ResetHome', 1);
        src = null;
    });

    $(document).on('click', '.btn_mode_absence', function () {
        raise('GotoAbsenceMode', 1);
        src = null;
    });

    $(document).on('click', '.btn_goto_normal_mode', function () {
        var password = $('.input_password').val();

        if (password == '1234') {
            raise('GotoNormalMode', 1);
        }
    });

    var intervalId = window.setInterval(function () {
        if (src !== null) {
            $('.iframe_camera_home').show();
            $('.iframe_camera_home').attr('src', function (i, val) { return src; });
        } else {
            $('.iframe_camera_home').hide();
        }
    }, 150);

});
