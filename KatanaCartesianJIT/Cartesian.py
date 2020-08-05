#!/usr/bin/env python
# encoding: utf-8

from Katana import Nodes3DAPI
from Katana import FnAttribute
from Katana import NodegraphAPI, GeoAPI, PyScenegraphAttr, FnAttribute

ScenegraphAttr = PyScenegraphAttr
import logging

log = logging.getLogger('Nodes3DAPI.Merge')


_ExtraHints = {
    'Merge': {
        'widget': 'multiInputPortNode'},
    'Merge.showAdvancedOptions': {
        'widget': 'boolean',
        'help': '\n        Enables advanced options useful for special cases in which the merged\n        hierarchies partially overlap.\n        '},
    'Merge.advanced': {
        'conditionalVisOp': 'equalTo',
        'conditionalVisPath': '../showAdvancedOptions',
        'conditionalVisValue': 'Yes'},
    'Merge.advanced.sumBounds': {
        'widget': 'boolean',
        'help': '\n        When enabled, <b>bound</b> attributes are queried for each relevant\n        input location and the unioned results are used.\n        '},
    'Merge.advanced.preserveWorldSpaceXform': {
        'widget': 'boolean',
        'help': '\n        When enabled, all inherited <b>xform</b> attributes (preceded by an origin\n        statement) are applied at each\n        location whose source input differs from that of its parent. This is only\n        necessary in exceptional situations where there are conflicting transformations\n        on overlapping locations of the merge inputs.\n        '},
    'Merge.advanced.preserveInheritedAttributes': {
        'widget': 'attributeNameArray',
        'help': "\n        A list of attribute names for which inheritance should be preserved when\n        choosing between inputs of the merge. Whenever a child location's source\n        input differs from that of its parent, these attributes will be queried\n        globally and applied locally the child location.\n        "},
    'Merge.advanced.preferredInputAttributes': {
        'widget': 'interwovenArrayGroup',
        'childHints': {
            'name': {
                'widget': 'attributeName'},
            'index': {
                'int': 'True'}},
        'help': '\n        Lists of attribute names and indices of inputs for which the preferred value\n        of an attribute should be read.\n        '},
    'Merge.advanced.preferRightmostInputWithAttribute': {
        'help': 'By default, if a location is present in more than one of the\n        input scenes, then attribute values are taken from the leftmost input\n        which has the location. This parameter allows attributes as well as\n        child locations of a location with an attribute of the given name to be\n        taken from the <i>rightmost</i> input which has the location.'},
    'Merge.advanced.mergeGroupAttributes': {
        'widget': 'attributeNameArray',
        'help': 'If the locations of multiple input scenes contain any group\n        attributes named in this parameter, their child attributes will be\n        merged instead of overwritten (unless otherwise stated by\n        <b>preferRightmostInputWithAttribute</b> or\n        <b>preferredInputAttributes</b>.'}}

_Parameter_XML = "\n<group_parameter>\n \n <string_parameter name='showAdvancedOptions' value='No'/>\n <group_parameter name='advanced'>\n   <string_parameter name='sumBounds' value='No'/>\n   <string_parameter name='preserveWorldSpaceXform' value='No'/>\n   <stringarray_parameter name='preserveInheritedAttributes' length='0'/>\n   \n   <group_parameter name='preferredInputAttributes'>\n    <stringarray_parameter name='name' length='0'/>\n    <numberarray_parameter name='index' length='0'/>\n    \n   </group_parameter>\n   \n   <string_parameter name='preferRightmostInputWithAttribute'/>\n   <stringarray_parameter name='mergeGroupAttributes' length='0'/>\n </group_parameter>\n\n</group_parameter>\n"
class MergeNode3D(Node3D.Node3D):

    def __init__(self):
            Node3D.Node3D.__init__(self)
            self.addOutputPort('out')
            self.getParameters().parseXML(_Parameter_XML)

    def getEmptyScene(self, port, frameTime):
        """Build a geometry producer which creates an almost-empty scenegraph.
           This contains the empty paths /root/world and /root/materials.
           This is called when we don't have any inputs."""
        rootAttrs = [
            ('name', ScenegraphAttr.Attr('StringAttr', [
                'root'], 1)),
            ('type', ScenegraphAttr.Attr('StringAttr', [
                'group'], 1))]
        worldAttrs = [
            ('name', ScenegraphAttr.Attr('StringAttr', [
                'world'], 1)),
            ('type', ScenegraphAttr.Attr('StringAttr', [
                'group'], 1))]
        materialsAttrs = [
            ('name', ScenegraphAttr.Attr('StringAttr', [
                'materials'], 1)),
            ('type', ScenegraphAttr.Attr('StringAttr', [
                'group'], 1))]
        rootObject = GeoAPI.CFilters.GeometryProducer(producerType='StaticGeometryProducer',
                                                      description=(rootAttrs, []), historyName=self.getName(),
                                                      variableName=self.getVariableName(port, frameTime))
        rootObject.addChild(worldAttrs)
        rootObject.addChild(materialsAttrs)
        return rootObject

    def _getOpChain(self, interface):
        graphState = interface.getGraphState()
        frameTime = graphState.getTime()
        interface.setExplicitInputRequestsEnabled(True)
        inputBehavior = interface.SKIP
        argsGb = FnAttribute.GroupBuilder()


        numValidInputs = 0
        for port in self.getInputPorts():
            interface.addInputRequest(port.getName(), graphState, invalidInputBehavior=inputBehavior)
            if port.getNumConnectedPorts():
                numValidInputs += 1
                continue
        opArgs = argsGb.build()
        if numValidInputs > 1 or opArgs.getNumberOfChildren() > 0:
            interface.appendOp('CartesianScript', opArgs)
        return None

    def addParameterHints(self, attrName, inputDict):
        inputDict.update(_ExtraHints.get(attrName, {}))
        if attrName.startswith('Merge.advanced.preferredInputAttributes.name.'):
            inputDict['widget'] = 'attributeName'
            inputDict['label'] = 'name'
        elif attrName.startswith('Merge.advanced.preferredInputAttributes.index.'):
            inputDict['int'] = 'True'
            inputDict['label'] = 'index'
        elif attrName.startswith('Merge.advanced.preserveInheritedAttributes.'):
            inputDict['widget'] = 'attributeName'



NodegraphAPI.RegisterPythonNodeType('CartesianScript', MergeNode3D)
NodegraphAPI.AddNodeFlavor('CartesianScript', '3d')
