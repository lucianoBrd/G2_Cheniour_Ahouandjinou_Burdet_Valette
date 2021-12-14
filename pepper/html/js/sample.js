$(document).ready(function () {
    session = new QiSession();

    session.service("ALMemory").done(function (ALMemory) {

        ALMemory.subscriber("JsonHome").done(function (subscriber) {

            subscriber.signal.connect(function (data) {
                const home = jQuery.parseJSON(data)

                if (home.label !== 'undefined') {
                    $('.home').html(home.label);
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
