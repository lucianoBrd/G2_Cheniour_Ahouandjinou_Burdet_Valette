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
    </Dialogs>
    <Resources>
        <File name="index" src="html/index.html" />
        <File name="jquery-1.11.2.min" src="html/jquery-1.11.2.min.js" />
        <File name="sample" src="html/js/sample.js" />
        <File name="api_joke" src="scripts/api.py" />
        <File name="readme" src="readme.md" />
        <File name="api" src="scripts/api.pyc" />
        <File name="service" src="scripts/service.py" />
    </Resources>
    <Topics>
        <Topic name="stop_frf" src="stop/stop_frf.top" topicName="stop" language="fr_FR" />
        <Topic name="home_frf" src="home/home_frf.top" topicName="home" language="fr_FR" />
        <Topic name="home_list_frf" src="home_list/home_list_frf.top" topicName="home_list" language="fr_FR" />
        <Topic name="choice_home_frf" src="choice_home/choice_home_frf.top" topicName="choice_home" language="fr_FR" />
        <Topic name="reset_home_frf" src="reset_home/reset_home_frf.top" topicName="reset_home" language="fr_FR" />
    </Topics>
    <IgnoredPaths />
    <Translations auto-fill="fr_FR">
        <Translation name="translation_fr_FR" src="translations/translation_fr_FR.ts" language="fr_FR" />
    </Translations>
</Package>
