<?xml version="1.0" encoding="UTF-8" ?>
<Package name="cpe_app_3" format_version="4">
    <Manifest src="manifest.xml" />
    <BehaviorDescriptions>
        <BehaviorDescription name="behavior" src="behavior_1" xar="behavior.xar" />
    </BehaviorDescriptions>
    <Dialogs>
        <Dialog name="stop" src="stop/stop.dlg" />
        <Dialog name="home" src="home/home.dlg" />
        <Dialog name="home_list" src="home_list/home_list.dlg" />
        <Dialog name="choice_home" src="choice_home/choice_home.dlg" />
        <Dialog name="reset_home" src="reset_home/reset_home.dlg" />
        <Dialog name="label_home" src="label_home/label_home.dlg" />
        <Dialog name="create_action" src="create_action/create_action.dlg" />
        <Dialog name="get_temperature" src="get_temperature/get_temperature.dlg" />
        <Dialog name="say_temperature" src="say_temperature/say_temperature.dlg" />
        <Dialog name="get_humidity" src="get_humidity/get_humidity.dlg" />
        <Dialog name="say_humidity" src="say_humidity/say_humidity.dlg" />
        <Dialog name="goto_normal_mode" src="goto_normal_mode/goto_normal_mode.dlg" />
        <Dialog name="absence_mode" src="absence_mode/absence_mode.dlg" />
        <Dialog name="goto_absence_mode" src="goto_absence_mode/goto_absence_mode.dlg" />
        <Dialog name="goto_fete_mode" src="goto_fete_mode/goto_fete_mode.dlg" />
        <Dialog name="normal_mode" src="normal_mode/normal_mode.dlg" />
        <Dialog name="mode" src="mode/mode.dlg" />
    </Dialogs>
    <Resources>
        <File name="index" src="html/index.html" />
        <File name="jquery-1.11.2.min" src="html/jquery-1.11.2.min.js" />
        <File name="sample" src="html/js/sample.js" />
        <File name="api_joke" src="scripts/api.py" />
        <File name="readme" src="readme.md" />
        <File name="service" src="scripts/service.py" />
        <File name="rire" src="sound/rire.ogg" />
        <File name="api" src="scripts/api.pyc" />
        <File name="default" src="html/css/default.css" />
        <File name="alerte" src="sound/alerte.ogg" />
    </Resources>
    <Topics>
        <Topic name="stop_frf" src="stop/stop_frf.top" topicName="stop" language="fr_FR" />
        <Topic name="home_frf" src="home/home_frf.top" topicName="home" language="fr_FR" />
        <Topic name="home_list_frf" src="home_list/home_list_frf.top" topicName="home_list" language="fr_FR" />
        <Topic name="choice_home_frf" src="choice_home/choice_home_frf.top" topicName="choice_home" language="fr_FR" />
        <Topic name="reset_home_frf" src="reset_home/reset_home_frf.top" topicName="reset_home" language="fr_FR" />
        <Topic name="label_home_frf" src="label_home/label_home_frf.top" topicName="label_home" language="fr_FR" />
        <Topic name="create_action_frf" src="create_action/create_action_frf.top" topicName="create_action" language="fr_FR" />
        <Topic name="get_temperature_frf" src="get_temperature/get_temperature_frf.top" topicName="get_temperature" language="fr_FR" />
        <Topic name="say_temperature_frf" src="say_temperature/say_temperature_frf.top" topicName="say_temperature" language="fr_FR" />
        <Topic name="get_humidity_frf" src="get_humidity/get_humidity_frf.top" topicName="get_humidity" language="fr_FR" />
        <Topic name="say_humidity_frf" src="say_humidity/say_humidity_frf.top" topicName="say_humidity" language="fr_FR" />
        <Topic name="goto_normal_mode_frf" src="goto_normal_mode/goto_normal_mode_frf.top" topicName="goto_normal_mode" language="fr_FR" />
        <Topic name="absence_mode_frf" src="absence_mode/absence_mode_frf.top" topicName="absence_mode" language="fr_FR" />
        <Topic name="goto_absence_mode_frf" src="goto_absence_mode/goto_absence_mode_frf.top" topicName="goto_absence_mode" language="fr_FR" />
        <Topic name="goto_fete_mode_frf" src="goto_fete_mode/goto_fete_mode_frf.top" topicName="goto_fete_mode" language="fr_FR" />
        <Topic name="normal_mode_frf" src="normal_mode/normal_mode_frf.top" topicName="normal_mode" language="fr_FR" />
        <Topic name="mode_frf" src="mode/mode_frf.top" topicName="mode" language="fr_FR" />
    </Topics>
    <IgnoredPaths />
    <Translations auto-fill="fr_FR">
        <Translation name="translation_fr_FR" src="translations/translation_fr_FR.ts" language="fr_FR" />
    </Translations>
</Package>
